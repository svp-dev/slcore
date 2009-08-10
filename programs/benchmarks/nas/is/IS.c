#include <svp/slr.h>
#include <svp/div.h>
#include <svp/assert.h>
#include <svp/iomacros.h>
#include <svp/perf.h>
//#include "buckets.1.128.h"
#define INT int
#define SIZE unsigned

//extern int num_buckets_log;  // defined in buckets.x.y.slh
int num_physical_cores;
#define NUM_BUCKETS (1 << num_buckets_log)

#define MAX_SUPPORTED_BUCKETS (1 << 12)  // !!!
#define MAX_SUPPORTED_BUCKETING_THREADS (1 << 12)

int block_size = 0;

//#define NUM_CORES_LOG 3
//#define NUM_CORES (1 << NUM_CORES_LOG)

int num_bucketing_threads;

int max_key_log;  // read from input file
//extern int max_key_log_generator;  // defined in buckets.x.y.slh
#define MAX_KEY (1 << max_key_log)

int num_buckets_log; //command line argument

#define MAX_SUPPORTED_KEY (1<<20) // !!!!
#define MAX_SUPPORTED_PHYSICAL_CORES (1<<7) // !!!!

//#define bucket_start(i) bucket_limits[i]
//#define bucket_end(i) bucket_limits[i+1]
#define bucket_start(i) (i * (1<<SHIFT_KEY_TO_BUCKET)) 
#define bucket_end(i) ((i+1) * (1<<SHIFT_KEY_TO_BUCKET))
//#define compute_num_buckets ((sizeof(bucket_limits)/sizeof(int)) - 1)
#define compute_num_buckets (1<<num_buckets_log)

#define SHIFT_KEY_TO_BUCKET (max_key_log - num_buckets_log)

INT* keys;
SIZE num_keys;

INT bucket_size[MAX_SUPPORTED_BUCKETING_THREADS][MAX_SUPPORTED_BUCKETS];  //!!! NUM_CORES
INT bucket_sizes[MAX_SUPPORTED_BUCKETS];
INT* bucket_ptrs[MAX_SUPPORTED_BUCKETS];
INT bucket_start[MAX_SUPPORTED_BUCKETING_THREADS][MAX_SUPPORTED_BUCKETS]; 
INT smaller_than_bucket[MAX_SUPPORTED_BUCKETS];
  //INT ranks[MAX_KEY];
INT ranks[MAX_SUPPORTED_KEY];


#define first_element(_1, _2, _3) ((divu((_1),(_3)) * (_2)) + (((_2) < (modu((_1), (_3)))) ? 1 : 0))
#define num_elements(_1, _2, _3) (divu((_1), (_3)) + (((_2) < (modu((_1), (_3)))) ? 1 : 0))

/*
#define first_element(num_elements, index, stripes) \
    ((int)(((float)num_elements / stripes) * (index) + \
     (index < (num_elements%stripes) ? index : num_elements%stripes)))

#define num_elements(no_elements, index, stripes) \
    ((int)((no_elements / stripes) + \
    (index < (no_elements % stripes) ? 1 : 0)))
*/

sl_decl(randlc, double, 
    sl_glparm(double*, X), 
    sl_glparm(double*, A),
    sl_glparm(double*, result));

double X = 314159265.00, A = 1220703125.00;

#define start_timing(_1) do { _1 = get_cycles(); } while(0)
#define stop_timing(_1) do { _1 = get_cycles() - _1; } while(0)

#define start_global_counter()						\
  do {									\
    if (!global_counter_started) {					\
      global_counter_started = 1;					\
      uninteresting_cycles = get_cycles();				\
    } else {								\
      uninteresting_cycles += get_cycles() - paused_global_counter;	\
    }									\
  }									\
  while(0)

#define pause_global_counter()			\
  do{						\
    paused_global_counter = get_cycles();	\
  } while(0)

#define global_counter (get_cycles() - uninteresting_cycles)

#define my_printf(Fmt, ...)			\
  do {						\
    pause_global_counter();			\
    printf(Fmt, ## __VA_ARGS__);		\
    start_global_counter();			\
  } while(0)

long uninteresting_cycles;
int global_counter_started = 0;
long paused_global_counter = 0;


sl_def(generate_key, void, sl_glparm(int*, result)) {
  int i = 0;
  double rez = 0;
  double key = 0;
  for (i = 0; i < 4; i++) {
    sl_create(,,0,1,1,,,randlc,
        sl_glarg(double*, X, &X),
        sl_glarg(double*, A, &A),
        sl_glarg(double*, result, &rez));
    sl_sync();
    key += rez;
  }
  int* result = sl_getp(result);
  *result = (int)(MAX_KEY/4 * key); 
  //if (iii < 10)
  //printf("key %d: %d\n", iii++, (int)(MAX_KEY/4 * key));
}
sl_enddef

// binary search. Result will contain the index of the last element of
// vec <= val. n is the size of vec.
// At step i, compute bit i of the result.
#define binary_search(val, vec, n, result) \
  do { \
    int step;  \
    result = 0; \
    for (step = 1; step < n; step <<= 1); \
    for (result = 0; step; step >>= 1) \
      if ((result | step) < n && vec[result + step] <= val) {\
        result |= step; /* same as result+=step */\ 
      } \
  } while(0)

sl_def(compute_bucket_sizes, void) {
  sl_index(myindex);
  int ii = myindex;
  int i = 0;
  INT* my_bucket_size = bucket_size[myindex];

  // TODO(andreimatei): how can i parallelize this?
  // Would a sequential create be more
  // efficient?
  int start = first_element(num_keys, ii, num_bucketing_threads);
  
  int tempxxx = num_elements(1,2,3);
  
  int size = num_elements(num_keys, ii, num_bucketing_threads);
  //printf("compute_bucket_sizes: index = %d   start = %d   size = %d   NUM_CORES = %d   num_keys = %d\n",
  //        ii, start, size,
  //        NUM_CORES, num_keys);

  int bucket;
  int num_buckets = compute_num_buckets;
  for (i = start; i < start + size; i++) {
                  // save bucket computer here?
    //binary_search(keys[i], bucket_limits, num_buckets + 1, bucket);
    bucket = (keys[i] >> SHIFT_KEY_TO_BUCKET);
    my_bucket_size[bucket]++;
  }
}
sl_enddef


sl_def(bucket_keys, void) {
  sl_index(index);
  int i = 0;
  int bucket;
  int num_buckets = compute_num_buckets;
  for (i = first_element(num_keys, index, num_bucketing_threads);
       i < first_element(num_keys, index, num_bucketing_threads)
            + num_elements(num_keys, index, num_bucketing_threads);
       i++) {
    bucket = keys[i] >> SHIFT_KEY_TO_BUCKET; // !!!!!!!!
    //binary_search(keys[i], bucket_limits, num_buckets + 1, bucket);
    // copy key to it's bucket
    bucket_ptrs[bucket][bucket_start[index][bucket]++] = keys[i];
  }
}
sl_enddef


sl_def(compute_ranks, void) {
  sl_index(index);
  int my_index = index;
  int i = 0;
  int* p;
  int bucket;
  int* limit;
  if (index < compute_num_buckets - 1) limit = bucket_ptrs[index+1];
  else limit = bucket_ptrs[index] + bucket_sizes[index];
  for (p = bucket_ptrs[index]; p < limit; p++) {
    ranks[*p]++;
  }
  int start_key = bucket_start(my_index);
  int end_key = bucket_end(my_index);

  int cur_rank = smaller_than_bucket[index];
  int temp;
  for (i = start_key + 1; i < end_key; i++) 
    if (ranks[i] != 0) {
      temp = ranks[i];
      ranks[i] = cur_rank;
      cur_rank += temp;
    }
}
sl_enddef

sl_def(do_compute_ranks, void) {//       sl_glparm(int, 
  sl_index(index);
  int my_index = index;
  int i = 0;
  int* p;
  int bucket;
  int* limit;
  if (index < compute_num_buckets - 1) limit = bucket_ptrs[index+1];
  else limit = bucket_ptrs[index] + bucket_sizes[index];
  for (p = bucket_ptrs[index]; p < limit; p++) {
    ranks[*p]++;
  }
  int start_key = bucket_start(my_index);
  int end_key = bucket_end(my_index);

  int cur_rank = smaller_than_bucket[index];
  int temp;
  for (i = start_key + 1; i < end_key; i++) 
    if (ranks[i] != 0) {
      temp = ranks[i];
      ranks[i] = cur_rank;
      cur_rank += temp;
    }
}
sl_enddef

sl_def(compute_ranks2, void) {
  sl_index(index);
  //int num_my_buckets = num_elements(NUM_BUCKETS, index, 
  //    num_physical_cores);
  sl_create(,,index,NUM_BUCKETS,num_physical_cores,,,
      do_compute_ranks);//, sl_glarg(int, startindex));
  sl_sync();
}
sl_enddef

// counts how many times each (present) key appears
// in a bucket (the keys have already been bucketed, so
// it won't appear in any other bucket)
sl_def(mark_present_keys, void) {
  sl_index(index);
  int my_index = index;
  int* p,*limit;
  if (index < compute_num_buckets - 1) limit = bucket_ptrs[index+1];
  else limit = bucket_ptrs[index] + bucket_sizes[index];
  for (p = bucket_ptrs[index]; p < limit; p++) {
    ranks[*p]++;
  }
}
sl_enddef


slr_decl(slr_var(int, data, "data to sort"),
         slr_var(int, block_size, "block size to use when creating "
           "families"),
         slr_var(int, physical_cores, "number of cores the simulator is being ran with"),
         slr_var(int, num_buckets_log, "log of number of buckets"));

sl_def(t_main, void) {
  int64_t p1, p2;
  int64_t computing_sizes_cycles, 
          bucketing_cycles, computing_ranks_cycles;

  if (slr_len(physical_cores)) 
    num_physical_cores = slr_get(physical_cores)[0];
  else 
    num_physical_cores = 1;
  if (slr_len(num_buckets_log))
    num_buckets_log = slr_get(num_buckets_log)[0];
  else
    num_buckets_log = 1;

  num_bucketing_threads = 256 * num_physical_cores;

  do {

  int* d = slr_get(data);
  unsigned l = slr_len(data);

  num_keys = l - 1;
  keys = d + 1;
  max_key_log = d[0];


  INT my_keys[4] = {1,2,3,4};
  keys = my_keys;
  num_keys = 4;

  /*
  //verify that the buckets.x.y.slh corresponds to the input file
  if (max_key_log != max_key_log_generator) {
    printf("max_key_log in buckets.x.y.slh does not corresponde to input file (%d vs %d)!!!!\n", max_key_log_generator, max_key_log);
    break;
  }
  */
  
  printf("num_keys: %d\t max_key: %d\t num_cores: %d\t num_buckets: %d\n",        num_keys, MAX_KEY, num_physical_cores, NUM_BUCKETS);

  start_global_counter();

  int i = 0, j = 0;

  int key = 0;

  INT keys2[num_keys];
  my_printf("Starting sort: cycles = %d\n", get_cycles());

  // compute the size of each bucket
  start_timing(computing_sizes_cycles);
  sl_create(,,0,num_bucketing_threads,1,,,compute_bucket_sizes);
  sl_sync();
  // aggregate the sizes computed by each thread and compute
  // the starting pointer for each bucket
  int smaller = 0;
  for (j = 0; j < NUM_BUCKETS; j++) {
    int smaller2 = 0;
    for (i = 0; i < num_bucketing_threads; i++) {
      bucket_sizes[j] += bucket_size[i][j];
      bucket_start[i][j] = smaller2;
      smaller2 += bucket_size[i][j];
    }
    bucket_ptrs[j] = keys2 + smaller;
    smaller_than_bucket[j] = smaller;
    smaller += bucket_sizes[j];
  }
  stop_timing(computing_sizes_cycles);
  my_printf("Computing sizes took %d cycles.\n", 
      computing_sizes_cycles);

  // compute standard deviation of bucket sizes
  double mean = 0;
  for (i = 0; i < NUM_BUCKETS; i++) 
    mean += bucket_sizes[i];
  mean = mean / NUM_BUCKETS;

  double stddev = 0;
  double dif = 0;
  for (i = 0; i < NUM_BUCKETS; i++) {
    dif = (bucket_sizes[i] - mean);
    dif = dif * dif;
    stddev += dif; 
  }
  //stddev = sqrt(stddev / NUM_BUCKETS);
  stddev /= NUM_BUCKETS;

  my_printf("Average bucket size: %d\t square of stddev: %d\n",
      (int)mean, (int)stddev);
  
  // now a thread t can write unsynchronized to a bucket k
  // in range bucket_start[t][k] ... 
  // bucket_start[t][k] + bucket_size[t][k]. Of course, these
  // have to be translated with bucket_ptrs[k]

  
  // copy the keys to keys2 by respecting the buckets
  start_timing(bucketing_cycles);
  sl_create(,,0,num_bucketing_threads,1,,,bucket_keys);
  sl_sync();
  keys = keys2;
  stop_timing(bucketing_cycles);
  my_printf("Bucketing the keys took %d cycles.\n", bucketing_cycles);

  // compute the ranks
  start_timing(computing_ranks_cycles);
  //sl_create(,,0,NUM_BUCKETS,1,,,compute_ranks);
  //sl_sync();
  
  sl_proccall(compute_ranks2);
  stop_timing(computing_ranks_cycles);
  my_printf("Computing the ranks took %d cycles.\n",
      computing_ranks_cycles);

  my_printf("Done: cycles = %d\t global counter = %d\n", 
      computing_sizes_cycles + bucketing_cycles + 
      computing_ranks_cycles, global_counter);
  printf("Total cycles (including printing) = %d\n", get_cycles());
  

  //puts("Ranks: ");
  //for (i = 0; i < 32; i++) printf("%d ", ranks[i]);
  //puts("\n");
  } while(0);
}
sl_enddef

