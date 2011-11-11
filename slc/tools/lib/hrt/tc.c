#include "sl_hrt.h"
#include <ucontext.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>


#define CACHE_LINE_SIZE 128  // in bytes

#define VM_BITS_PER_NODE 40
#define VM_PER_NODE (1L<<VM_BITS_PER_NODE)
#define VM_BITS_PER_TC (25)
#define VM_PER_TC (1L<<VM_BITS_PER_TC)
//#define VM_RUNTIME (1L<<25)  // vm for runtime (on each node)
#define VM_TC_CONTROL_STRUCTURES (1L<<20)  // vm for TC control structures,
                                           // available at the beginning of each TC

#define NODE_START_VM (node_start_addr)
#define TC_START_VM(index) (NODE_START_VM + VM_PER_TC + (index)*VM_PER_TC)
                            // first VM_PER_TC is left for the runtime

#define RT_START_VM (node_start_addr)
#define RT_END_VM (node_start_addr + VM_PER_TC - 1)  // RT's vm is equal to a TC's vm
#define PROCESSOR_PTHREAD_STACK_SIZE (1<<15)

#define NO_FAM_CONTEXTS_PER_PROC 1024

//const long NODE_INDEX = 16L;  // TODO
#define NODE_INDEX 16L
void* node_start_addr = (void*)(NODE_INDEX * VM_PER_NODE);
int NO_PROCS = 2;  // the number of processors that will be created and used on the current node


tc_t* TC_START_VM_EXTENDED(int node_index, int tc_index) {
  assert(node_index == NODE_INDEX); // TODO
  //TODO: this function will have to look in some table with the start of the address space
  // for all nodes
  return TC_START_VM(tc_index); //TODO: this will have to be removed, see above
}

extern inline void st_rel_istruct(volatile i_struct* istruct, enum istruct_state value) {
  // TODO: store|store -> nop
  // TODO: load|store  -> nop
  __asm__ __volatile__("" : : : "memory");
  istruct->state = value;
  __asm__ __volatile__("" : : : "memory");
  // TODO: store|load => this one is _not_ a no-op on x86 BUT is only needed for seq consist, not for
  // release semantics... right?
}

extern inline enum istruct_state ld_acq_istruct(volatile i_struct* istruct) {
  enum istruct_state rez;
  __asm__ __volatile__("" : : : "memory");
  rez = istruct->state;
  __asm__ __volatile__("" : : : "memory");
  //TODO: load|load -> nop
  //TODO: load|store -> nop
  return rez;
}

enum LOG_LEVEL {CRASH = 0, WARNING = 3, INFO = 7, VERBOSE = 10, DEBUG = 15};
typedef enum LOG_LEVEL LOG_LEVEL;

#ifdef LOGGING
LOG_LEVEL logging_level = LOGGING;
#else
LOG_LEVEL logging_level = DEBUG;
#endif

void LOG(LOG_LEVEL level, char* fmt, ...) {
  if (level <= logging_level) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
  }
}

/*
struct tc_node {
  struct tc_node* next;
};
typedef struct tc_node tc_node;
tc_node
*/
typedef union {
  pthread_spinlock_t lock;
  char c[CACHE_LINE_SIZE];
} padded_spinlock_t;

tc_t* runnable_tcs[MAX_PROCS_PER_NODE][NO_TCS_PER_PROC];
int runnable_count[MAX_PROCS_PER_NODE];
padded_spinlock_t runnable_queue_locks[MAX_PROCS_PER_NODE];
padded_spinlock_t allocate_tc_locks[MAX_PROCS_PER_NODE];
pthread_t threads[MAX_PROCS_PER_NODE];
pthread_attr_t threads_attr[MAX_PROCS_PER_NODE];
__thread tc_t* cur_tc = NULL;  // pointer to the TC currently occupying the accessing processor
              // FIXME: check out where these things are allocated... if it causes mmapings of
              // regions that we have no control over, replace it with a macro that uses the SP
              // to get access to the TC




fam_context_t fam_contexts[MAX_PROCS_PER_NODE][NO_FAM_CONTEXTS_PER_PROC];
/* locks for allocating fam contexts; one per processor */
// TODO: pad to one per cache line
pthread_spinlock_t fam_contexts_locks[MAX_PROCS_PER_NODE];

void unblock_tc(tc_t* tc, int same_processor);
void block_tc_and_unlock(tc_t* tc, pthread_spinlock_t* lock);
void suspend_on_istruct(volatile i_struct* istructp, int same_proc);
void populate_tc(tc_t* tc,
                 thread_func func,
                 int num_shareds, int num_globals,
                 long start_index,
                 long end_index,
                 fam_context_t* fam_context,
                 tc_ident_t parent, tc_ident_t prev, tc_ident_t next,
                 int is_last_tc);
int atomic_increment_next_tc(int proc_id);

/*
 * Check wether a particular TC has been created on this node or not.
 * This function should be used whenever we iterate blindly through TCs.
 *
 *
 * *) some TCs might not have been created due to wholes in the address space.
 */
int tc_is_valid(int tc_index) {
  //TODO: actually check that the TC has been created
  return 1;
}

mapping_decision map_fam(
    thread_func func,
    long no_threads,
    //long start_index,
    //long end_index,
    struct mapping_node_t* parent_id) {
  assert(parent_id == NULL); // TODO
  mapping_decision rez;

  rez.should_inline = 0;
  rez.no_proc_assignments = 1;
  rez.proc_assignments[0].node_index = NODE_INDEX;
  rez.proc_assignments[0].proc_index = (cur_tc->ident.proc_index + 1) % NO_PROCS;
  rez.proc_assignments[0].no_tcs = 1;
  rez.proc_assignments[0].load_percentage = 100; // 100% of threads on this proc

  return rez;
}

fam_context_t* allocate_fam(
    thread_func func,
    int num_shareds, int num_globals,
    long start_index,
    long end_index,
    long step,
    struct mapping_node_t* parent_id,
    const struct mapping_decision* mapping) {

  LOG(DEBUG, "allocate_fam\n");
  assert(mapping != NULL);
  //find an empty fam_context within the family contexts of the current proc
  //(technically, it doesn't matter where the fam context is chosen from)
  int i;
  pthread_spin_lock(&fam_contexts_locks[cur_tc->ident.proc_index]);
  for (i = 0; i < NO_FAM_CONTEXTS_PER_PROC; ++i) {
    if (fam_contexts[cur_tc->ident.proc_index][i].empty) break;
  }
  assert(i < NO_FAM_CONTEXTS_PER_PROC); //TODO: return FAIL value
  fam_context_t* fc = &fam_contexts[cur_tc->ident.proc_index][i];
  fc->empty = 0;
  fc->done.state = EMPTY;
  pthread_spin_unlock(&fam_contexts_locks[cur_tc->ident.proc_index]);

  //find TCs
  int no_ranges = 0;
  long total_threads = (end_index - start_index + 1) / step;
  long allocated_threads = 0;
  long backlog = 0;
  long thread_index = -1; //start_index - 1;  // last allocated thread

  // first assign the threads as if the start index was 0 and step was 1, later we'll fix them
  for (i = 0; i < mapping->no_proc_assignments; ++i) {
    int j;
    proc_assignment as = mapping->proc_assignments[i];
    assert(as.node_index == NODE_INDEX);  //TODO

    int threads_for_proc;
    if (i != mapping->no_proc_assignments - 1)
      threads_for_proc = as.load_percentage * total_threads / 100;
    else
      threads_for_proc = total_threads - allocated_threads;

    if (threads_for_proc == 0) continue;
    int threads_for_tc = threads_for_proc / as.no_tcs;
    int threads_for_tc_last = threads_for_tc + threads_for_proc % as.no_tcs;

    for (j = 0; j < as.no_tcs; ++j) {
      int num_threads = (j < as.no_tcs - 1 ? threads_for_tc : threads_for_tc_last);
      int tc = atomic_increment_next_tc(as.proc_index);
      if (tc != -1) {
        if (backlog == 0) {
          fc->ranges[no_ranges].index_start = thread_index + 1;
          thread_index = fc->ranges[no_ranges].index_start + num_threads - 1;
          fc->ranges[no_ranges].index_end = thread_index;
        } else {  // this implies that this is the first range we're actually allocating
          fc->ranges[no_ranges].index_start = start_index;
          thread_index = start_index + backlog + num_threads - 1;
          fc->ranges[no_ranges].index_end = thread_index;
          backlog = 0;  // reset the backlog, as all the threads have been assigned to this range
        }
        fc->ranges[no_ranges].dest.node_index = as.node_index;
        fc->ranges[no_ranges].dest.proc_index = as.proc_index;
        fc->ranges[no_ranges].dest.tc_index = tc;
        fc->ranges[no_ranges].dest.tc = (tc_t*)TC_START_VM_EXTENDED(as.node_index, tc);

        ++no_ranges;
      } else {  // couldn't allocate a TC
        thread_index += num_threads;
        if (no_ranges > 0) {  // there has been at least a tc allocated so far
          // put the threads in the previous range
          fc->ranges[no_ranges - 1].index_end += num_threads;
        } else {  // put the threads in the backlog
          backlog += num_threads;
        }
      }
    }
  }

  // fix ranges; they are currently like start_index was 0 and step was 1; we need to translate and scale
  for (i = 0; i < no_ranges; ++i) {
    long old_start_index = fc->ranges[i].index_start;
    long old_end_index = fc->ranges[i].index_end;
    fc->ranges[i].index_start = start_index + (old_start_index) * step;
    fc->ranges[i].index_end = start_index + (old_end_index + 1) * step - 1;
  }

  if (no_ranges == 0) {
    assert(0); //TODO: return FAIL
  }
  fc->no_ranges = no_ranges;

  for (i = 0; i < no_ranges; ++i) {
    populate_tc(fc->ranges[i].dest.tc,
                func, num_shareds, num_globals,
                fc->ranges[i].index_start,
                fc->ranges[i].index_end,
                fc,
                cur_tc->ident, // parent
                i == 0 ? cur_tc->ident : fc->ranges[i-1].dest, // prev
                i == no_ranges - 1 ? cur_tc->ident : fc->ranges[i+1].dest, // next
                (i == no_ranges - 1)  // is_last_tc
                );
  }

  return fc;
}

/* Allocate a family for calling fam_main */
fam_context_t* allocate_fam_main(thread_func func) {

  //find empty TCs and fam_context
  int dest_proc = 0;
  assert(fam_contexts[0][0].empty);
  fam_context_t* fc = &fam_contexts[dest_proc][0];
  fc->empty = 0;
  fc->no_ranges = 1;
  fc->ranges[0].index_start = 0;
  fc->ranges[0].index_end = 0;
  fc->ranges[0].dest.proc_index = dest_proc;
  fc->ranges[0].dest.node_index = NODE_INDEX;//cur_tc->ident.node_index;
  fc->ranges[0].dest.tc_index = atomic_increment_next_tc(dest_proc);
  assert(fc->ranges[0].dest.tc_index != -1);
  fc->ranges[0].dest.tc = (tc_t*)TC_START_VM(fc->ranges[0].dest.tc_index);

  tc_ident_t dummy_parent;
  dummy_parent.node_index = -1; // no parent

  populate_tc(fc->ranges[0].dest.tc,
              func, 0, 0,
              fc->ranges[0].index_start,
              fc->ranges[0].index_end,
              fc,
              dummy_parent, dummy_parent, dummy_parent, 1);
  return fc;
}


/*--------------------------------------------------
* inline int test_same_node(tc_t* l, tc_t* r) {
*   return l->ident.node_index == r->ident.node_index;
* }
* inline int test_same_proc(tc_t* l, tc_t* r) {
*   return test_same_node(l,r) && l->ident.proc_index == r->ident.proc_index;
* }
* inline int test_same_tc(tc_t* l, tc_t* r) {
*   return test_same_proc(l, r) && l->ident.tc_index == r->ident.tc_index;
* }
*--------------------------------------------------*/
static inline int test_same_node(const tc_ident_t* l, const tc_ident_t* r) {
  return l->node_index == r->node_index;
}
static inline int test_same_proc(const tc_ident_t* l, const tc_ident_t* r) {
  return test_same_node(l,r) && l->proc_index == r->proc_index;
}
static inline int test_same_tc(const tc_ident_t* l,const tc_ident_t* r) {
  return test_same_proc(l, r) && l->tc_index == r->tc_index;
}

/* 
 * Unblocks all the tc-s that have been assigned chunks of the family.
 * Returns an identifier of the first tc to service the family.
 */
tc_ident_t create_fam(fam_context_t* fc) {
  //assert(fc->no_ranges == 1); // TODO
  assert(cur_tc == NULL || test_same_node(&fc->ranges[0].dest, &cur_tc->ident)); //TODO
  for (int i = 0; i < fc->no_ranges; ++i) {
    unblock_tc(fc->ranges[i].dest.tc,
               0 /*TODO: check if it is the same processor and modify this arg*/);
  }
  return fc->ranges[0].dest;
}


long sync_fam(fam_context_t* fc, /*long* shareds_dest,*/ int no_shareds, ...) {
  assert(test_same_node(&cur_tc->ident, &fc->ranges[fc->no_ranges-1].dest));  //TODO
  int same_proc = test_same_proc(&cur_tc->ident, &fc->ranges[fc->no_ranges-1].dest);
  assert(!test_same_tc(&cur_tc->ident, &fc->ranges[fc->no_ranges-1].dest));  // parent and
            // child should never be in the same tc
  suspend_on_istruct(&fc->done, same_proc);
  // copy the shareds to the parent
  int i;
  va_list ap;
  va_start(ap, no_shareds);
  for (i = 0; i < no_shareds; ++i) {
    *(va_arg(ap, long*)) = fc->shareds[i]; //fc->shareds[i].data;
    //shareds_dest[i] = fc->shareds[i].data;
  }
  va_end(ap);
  fc->empty = 1;  // mark the FC as reusable
  return 0;  // TODO: what exactly is this return code supposed to be?
}

void mmap_tc_ctrl_struct(int tc_index) {
  void* addr = TC_START_VM(tc_index);
  assert(!((unsigned long)addr % getpagesize()));  // must be page-aligned
  int no_pages = sizeof(tc_t) / getpagesize();
  ++no_pages;
  void* mapping = mmap(addr, no_pages * getpagesize(), PROT_READ | PROT_WRITE,
                       MAP_PRIVATE|MAP_FIXED|MAP_ANON, -1, 0);
  if (mapping == MAP_FAILED) {
    perror("mmap"); exit(1);
  }
  LOG(DEBUG + 1, "mapped %d pages starting from %p for a TC control structure\n",
      no_pages, addr);
}

void run_tc(int processor_index, tc_t* tc) {
  //int jumped = 0;
  assert(tc->ident.proc_index == processor_index);

  cur_tc = tc;
  LOG(DEBUG, "jumping to user code\n");
  swapcontext(&processor[processor_index].scheduler_context, &tc->context);
  LOG(DEBUG, "back from user code\n");

  /*
  getcontext(&processor[processor_index].scheduler_context);
  LOG(DEBUG, "just after getcontext()\n");
  if (!jumped) {
    jumped = 1;
    tc->context.uc_link = &processor[processor_index].scheduler_context;
          // make sure we come back to this function if the thread function finishes execution
    LOG(VERBOSE, "Jumping to thread func\n");
    setcontext(&(tc->context));
  } else {
    // we executed the TC and came back;
    LOG(VERBOSE, "Came back to run_tc\n");
    // check if the threads terminated or just yielded; if they terminated, mark the TC as empty
    if (tc->finished == 1) {
      tc->blocked = -1;  // empty
    }
    */

    //return to the scheduler
    return;
  //}
}


void* run_processor(void* processor_index) {
  LOG(INFO, "Starting processor %ld\n", (unsigned long)processor_index);
  int pid = (unsigned long)processor_index;
  while (1) {
    pthread_spin_lock((pthread_spinlock_t*)&runnable_queue_locks[pid]);
    while (runnable_count[pid] == 0) {
      pthread_spin_unlock((pthread_spinlock_t*)&runnable_queue_locks[pid]);
      usleep(1000);
      pthread_spin_lock((pthread_spinlock_t*)&runnable_queue_locks[pid]);
    }
    // TODO: implement a proper queue for these and see how to do without locking for
    // extracting
    tc_t* tc = runnable_tcs[pid][0];
    int i;
    for (i = 0; i < runnable_count[pid] - 1; ++i) {
      runnable_tcs[pid][i] = runnable_tcs[pid][i+1];
    }
    --runnable_count[pid];
    pthread_spin_unlock((pthread_spinlock_t*)&runnable_queue_locks[pid]);

    // run tc
    LOG(DEBUG, "run_processor: found runnable TC\t proc index = %d\n", processor_index);
    run_tc(pid, tc);
  }
}

/*
 * Map vm for a processor stack. The stacks are located at the end of the runtime's vm, with
 * the stack for the first processor located at lower addresses, then the stack for the 2nd processor,
 * and so on.
 */
void* mmap_processor_stack(int processor_index) {
  void* low_addr = RT_END_VM - (NO_PROCS - processor_index)*PROCESSOR_PTHREAD_STACK_SIZE + 1;
  assert(((unsigned long)low_addr % getpagesize()) == 0);
  void* mapping = mmap(low_addr, PROCESSOR_PTHREAD_STACK_SIZE, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE|MAP_FIXED|MAP_ANON, -1, 0);
  //TODO: add a guard page between stacks
  if (mapping == MAP_FAILED) {
    perror("mmap"); exit(1);
  }
  assert(mapping == low_addr);
  return low_addr;
}

void init_processors() {
  unsigned long i;
  for (i = 0; i < NO_PROCS; ++i) {
    pthread_attr_init(&threads_attr[i]);
    void* stack_low_addr = mmap_processor_stack(i);
    if (pthread_attr_setstack(&threads_attr[i], stack_low_addr, PROCESSOR_PTHREAD_STACK_SIZE) != 0) {
      perror("attr_setstack"); exit(1);
    }
    if (pthread_create(&threads[i], &threads_attr[i], run_processor, (void*)i)) {
      perror("init_processors:"); exit(1);
    }
  }
}

void create_tc(int tc_index);

void rt_init() {
  /* sanity checks */
  assert(VM_PER_NODE >= NO_TCS_PER_PROC * MAX_PROCS_PER_NODE * VM_PER_TC);
  assert(RT_START_VM >= NODE_START_VM);
  assert(NODE_START_VM + VM_PER_NODE > TC_START_VM(NO_TCS_PER_PROC * MAX_PROCS_PER_NODE));
  
  //check that the lock within a padded lock is placed at the beginning of the union. We care, since we
  //cast the union to the lock.
  assert((void*)&allocate_tc_locks[0].lock == (void*)&allocate_tc_locks[0].c[0]);
  assert((void*)&allocate_tc_locks[0].lock == (void*)&allocate_tc_locks[0]);


  //init runnable_queue_locks
  int i, j;
  for (i = 0; i < NO_PROCS; ++i) {
    if (pthread_spin_init((pthread_spinlock_t*)&runnable_queue_locks[i], PTHREAD_PROCESS_PRIVATE) != 0) {
      perror("pthread_spin_init:"); exit(1);
    }
    if (pthread_spin_init(&fam_contexts_locks[i], PTHREAD_PROCESS_PRIVATE) != 0) {
      perror("pthread_spin_init:"); exit(1);
    }
    if (pthread_spin_init(((pthread_spinlock_t*)&allocate_tc_locks[i]), PTHREAD_PROCESS_PRIVATE) != 0) {
      perror("pthread_spin_init:"); exit(1);
    }
    /*
    for (j = 0; j < NO_FAM_CONTEXTS_PER_PROC; ++j) {
      if (pthread_spin_init(&fam_contexts[i][j].lock, PTHREAD_PROCESS_PRIVATE) != 0) {
        perror("pthread_spin_init:"); exit(1);
      }
    }
    */
  }
  
  // init TCs
  for (i = 0; i < NO_PROCS * NO_TCS_PER_PROC; ++i) {
    // mmap TC control structures
    mmap_tc_ctrl_struct(i);

    // init tc fields
    create_tc(i);
  }

  //init fam_contexts
  for (i = 0; i < NO_PROCS; ++i) {
    for (j = 0; j < NO_FAM_CONTEXTS_PER_PROC; ++j) {
      fam_contexts[i][j].empty = 1;
      fam_contexts[i][j].done.state = EMPTY;
      int k = 0;
      if (pthread_spin_init(&fam_contexts[i][j].done.lock, PTHREAD_PROCESS_PRIVATE) != 0) {
        perror("pthread_spin_init:"); exit(1);
      }
      for (k = 0; k < MAX_ARGS_PER_FAM; ++k) {
        /*
        if (pthread_spin_init(&fam_contexts[i][j].shareds[k].lock, PTHREAD_PROCESS_PRIVATE) != 0) {
          perror("pthread_spin_init:"); exit(1);
        }
        */
        /*
        if (pthread_spin_init(&fam_contexts[i][j].globals[k].lock, PTHREAD_PROCESS_PRIVATE) != 0) {
          perror("pthread_spin_init:"); exit(1);
        }
        */
      }
    }
  }

  // init processors
  init_processors();
}


static inline tc_ident_t get_current_context_ident() {
  tc_ident_t ident;
  unsigned long sp;
  __asm__ __volatile__ ("movq %%rsp, %0\n" : "=r" (sp));
  unsigned long node_vm_start =
    sp & ((0xffffffffffffffffL >> VM_BITS_PER_NODE) << VM_BITS_PER_NODE);
  ident.node_index = node_vm_start >> VM_BITS_PER_NODE;
  ident.tc = (void*)
    (sp & ((0xffffffffffffffffL >> VM_BITS_PER_TC) << VM_BITS_PER_TC));

  //ident.tc_index = ((unsigned long)ident.tc - node_vm_start) / VM_PER_TC;
  ident.tc_index = ((unsigned long)ident.tc - (unsigned long)TC_START_VM(0)) / VM_PER_TC;

  ident.proc_index = ident.tc_index / NO_TCS_PER_PROC;
  return ident;
}

void idle() {
  while(1) {}
}

void TC_finished() {
  printf("TC finished! looping...\n");
  while (1) {}
}

stack_t alloc_stack(int tc_index, int size) {
  void* stack_highaddr = (void*)TC_START_VM(tc_index + 1) - 1;  // start just before the next TC
  int num_pages = size / getpagesize();
  if (size % getpagesize()) num_pages++;
  size = num_pages * getpagesize();
  void* stack_lowaddr = stack_highaddr - size + 1;
  void* mapping =
    mmap(stack_lowaddr, size, PROT_READ | PROT_WRITE,
         MAP_PRIVATE|MAP_FIXED|MAP_ANON|
#if defined(MAP_GROWSDOWN) /* linux */
   MAP_GROWSDOWN
#elif defined(MAP_STACK) /* FreeBSD */
   MAP_STACK
#else
#error Cannot request stack-like mmap on this system.
#endif
    , -1, 0);
  if (mapping == MAP_FAILED) {
    perror("alloc_stack"); exit(1);
  }
  assert(mapping == stack_lowaddr);
  LOG(DEBUG + 1, "mapped %d pages starting from %p -> %p for a TC stack\n",
      num_pages, stack_lowaddr, stack_highaddr);
  stack_t stack;
  stack.ss_sp = stack_lowaddr;

  *((int*)stack.ss_sp) = 4;  //FIXME: remove this, just a test

  stack.ss_size = size;
  return stack;
}

heap_t alloc_heap(int tc_index, int size) {
  void* heap_lowaddr = (void*) TC_START_VM(tc_index) + VM_TC_CONTROL_STRUCTURES;
  int num_pages = size / getpagesize();
  if (size % getpagesize()) num_pages++;
  size = num_pages * getpagesize();
  void* mapping =
    mmap(heap_lowaddr, size, 0, MAP_PRIVATE|MAP_FIXED|MAP_ANON, -1, 0);
  if (mapping == MAP_FAILED) {
    perror("alloc_heap"); exit(1);
  }
  assert(mapping == heap_lowaddr);
  heap_t heap;
  heap.lowaddr = heap_lowaddr;
  heap.size = size;
  return heap;
}

void create_tc(int tc_index) {
  tc_t* tc = (tc_t*)TC_START_VM(tc_index);//&(TCS[tc_index];
  //get context
  //ucontext_t contextt;
  //getcontext(&contextt);
  getcontext(&tc->context);

  //allocate stack
  tc->initial_thread_stack = alloc_stack(tc_index, INIT_TC_STACK_SIZE);
  tc->context.uc_stack = tc->initial_thread_stack;
  tc->context.uc_link = NULL;

  //allocate heap
  tc->heap = alloc_heap(tc_index, INIT_TC_HEAP_SIZE);

  //makecontext
  makecontext(&(tc->context), idle, 0);

  //write ->ident
  tc->ident.node_index = NODE_INDEX;
  tc->ident.tc = tc;
  tc->ident.tc_index = tc_index;
  tc->ident.proc_index = tc_index / NO_TCS_PER_PROC;

  //init shared_locks and global_locks
  int i;
  for (i = 0; i < MAX_ARGS_PER_FAM; ++i) {
    if (pthread_spin_init(&tc->shareds[i].lock, PTHREAD_PROCESS_PRIVATE) != 0) {
      perror("pthread_spin_init:"); exit(1);
    }
    if (pthread_spin_init(&tc->globals[i].lock, PTHREAD_PROCESS_PRIVATE) != 0) {
      perror("pthread_spin_init:"); exit(1);
    }
  }

  tc->blocked = -1; // empty
  tc->finished = 1;  // available for reuse
}


/*
void push_to_TC_stack_ul(stack_t* stack, unsigned long data) {
  unsigned long* p = ((void*)stack->ss_sp + stack->ss_size) - sizeof(unsigned long);
  *p = data;
  //stack->ss_sp -= sizeof(unsigned long);
  stack->ss_size -= sizeof(unsigned long);

  LOG(DEBUG, "pushed arg at %p\n", p);
}
*/

/* prepares a TC to run a threads range. The TC is not scheduled to be run yet; that can be done manually 
 * through unblock_tc() or, generally, through create_fam
 */
void populate_tc(tc_t* tc,
                 thread_func func,
                 int num_shareds, int num_globals,
                 long start_index,
                 long end_index,
                 fam_context_t* fam_context,
                 tc_ident_t parent, tc_ident_t prev, tc_ident_t next,
                 int is_last_tc) {
  LOG(VERBOSE, "populate_tc - thread function: %p\n", func);
  tc->context.uc_stack = tc->initial_thread_stack;  // TODO: is this necessary? would this have been modified
                                                    // by savecontext() calls?
  tc->context.uc_link = NULL;
  LOG(DEBUG, "populate_tc: reset stack to %p\n", tc->initial_thread_stack.ss_sp);
  makecontext(&(tc->context), (void (*)())func, 0);

  /*
  // put start_index and end_index on the family's stack
  push_to_TC_stack_ul(&tc->context.uc_stack, end_index);
  push_to_TC_stack_ul(&tc->context.uc_stack, start_index);
  */

  tc->index_start = start_index;
  tc->index_stop = end_index;
  tc->fam_context = fam_context;
  tc->parent_ident = parent;
  tc->prev = prev; tc->next = next;
  tc->is_last_tc = is_last_tc;
  for (int i = 0; i < num_shareds; ++i) tc->shareds[i].state = EMPTY;
  for (int i = 0; i < num_globals; ++i) tc->globals[i].state = EMPTY;
  tc->finished = 0;
}



/* writer and reader are running on the same proc, but on different TCs.
   potentially_blocked_tc is the TC that might be suspended on reading this istruct.
   In the current implementation, there's no reason while the istruct needs to be volatile in this
   case, as it's not read twice,
 */
void write_istruct_same_proc(
    volatile i_struct* istructp,
    long val,
    tc_t* potentially_blocked_tc) {
  istructp->data = val;
  int unblock_needed = (istructp->state == SUSPENDED);
  istructp->state = WRITTEN;
  if (unblock_needed) {
    unblock_tc(potentially_blocked_tc, 1 /*same proc*/);
  }
}

/* Reader and writer run on different procs */
extern void write_istruct_different_proc(
    volatile i_struct* istructp,
    long val,
    tc_t* potentially_blocked_tc) {
  istructp->data = val;
  enum istruct_state istruct = ld_acq_istruct(istructp);
  assert(istruct != WRITTEN);
  if (istruct == SUSPENDED) {
    // no locking necessary
    st_rel_istruct(istructp, WRITTEN);
    unblock_tc(potentially_blocked_tc,
               0 /*different processor*/);
  } else {
    // the target tc is either running or blocked on a different istruct or smth else => locking is needed
    pthread_spin_lock(&istructp->lock);
    istruct = ld_acq_istruct(istructp);
    int must_unblock = (istruct == SUSPENDED);
    st_rel_istruct(istructp, WRITTEN);
    pthread_spin_unlock(&istructp->lock);
    if (must_unblock) {
      unblock_tc(potentially_blocked_tc, 0 /*different processor*/);
    }
  }
}

/* wrapper function for reading an istruct when we don't know whether the reader and the writer
   are running on the same proc or TC.
   reading_tc corresponds to the TC that will (or already has) read the istruct
 */
void write_istruct(volatile i_struct* istructp, long val, const tc_ident_t* reading_tc) {
  tc_ident_t cur_ident = get_current_context_ident();

  assert(cur_ident.node_index == cur_tc->ident.node_index);  //TODO: remove these
  assert(cur_ident.proc_index == cur_tc->ident.proc_index);
  assert(cur_ident.tc_index == cur_tc->ident.tc_index);

  assert(reading_tc->node_index == cur_ident.node_index);  // assume same node, for now
  tc_t* dest_tc = (tc_t*)reading_tc->tc;
  if (reading_tc->tc_index == cur_ident.tc_index) {  // same thread context
    write_istruct_same_tc((i_struct*)istructp, val);  // cast to strip volatile; same TC, no asynchrony
  } else {
    if (reading_tc->proc_index == cur_ident.proc_index) {  // same proc
      write_istruct_same_proc(istructp, val, dest_tc);
    }
    else {  // different proc
      write_istruct_different_proc(istructp, val, dest_tc);
    }
  }
}

/* handles same or different proc*/
long read_istruct_different_tc(volatile i_struct* istruct, int same_proc) {
  suspend_on_istruct(istruct, same_proc);
  assert(istruct->state == WRITTEN);
  //istruct->state = EMPTY;
  return istruct->data;
}

long read_istruct(volatile i_struct* istructp, const tc_ident_t* writing_tc) {
  istruct_state state = ld_acq_istruct(istructp);
  if (state == WRITTEN) {  // fast path: the istruct has been filled already
    //istructp->state = EMPTY;
    return istructp->data;
  }

  //both this function and the function called below implement a fast path for when the istruct
  //has already been written. The called functions are also called by the user code directly, so
  //we don't want to remove this double checking.

  if (!test_same_node(writing_tc, &cur_tc->ident)) {
    //TODO: different node
    assert(0);
    return -1;
  }
  if (test_same_tc(&cur_tc->ident, writing_tc)) {
    return read_istruct_same_tc((i_struct*)istructp);  // cast to strip volatile
  } else {
    return read_istruct_different_tc(istructp, test_same_proc(writing_tc, &cur_tc->ident));
  }
}

static inline istruct_state read_istruct_state(volatile i_struct* istructp, int same_proc) {
  if (!same_proc) {
    return ld_acq_istruct(istructp);
  } else {
    return istructp->state;
  }
}

static inline void write_istruct_state(volatile i_struct* istructp, int same_proc, istruct_state value) {
  if (!same_proc) {
    st_rel_istruct(istructp, value);
  } else {
    istructp->data = value;
  }
}

/* Reads an istruct and suspends the TC if it is found to the empty.

   same_proc is a hint; pass 1 if the writer is known to be running on the same proc;
   that allows some optimizations (not taking the lock);
 */
void suspend_on_istruct(volatile i_struct* istructp, int same_proc) {
  // fast path
  istruct_state state = read_istruct_state(istructp, same_proc);
  if (state == WRITTEN) {
    return;
  }
  // slow path
  if (!same_proc) {pthread_spin_lock(&istructp->lock);}
  state = read_istruct_state(istructp, same_proc);
  if (state != WRITTEN) {
    write_istruct_state(istructp, same_proc, SUSPENDED);
    // we sleep
    block_tc_and_unlock(cur_tc, same_proc ? NULL : &istructp->lock);
    // ... and we wake up
  } else {
    if (!same_proc) pthread_spin_unlock(&istructp->lock);
  }
}

void announce_runnable_not_empty(int proc_index) {
  //FIXME
  //assert(0);
}

#define PRINT_TC_IDENT(tc) tc.node_index, tc.proc_index, tc.tc_index
#define PRINT_TC_IDENT_FORMAT "N=%d P=%d TC=%d"

// same_processor = 1, if the tc to be unblocked is handled by the same processor as the caller
void unblock_tc(tc_t* tc, int same_processor) {
  pthread_spinlock_t* lock;
  LOG(VERBOSE, "unblocking TC ("PRINT_TC_IDENT_FORMAT")\n", PRINT_TC_IDENT(tc->ident));
  if (!same_processor) {
    lock = (pthread_spinlock_t*)&runnable_queue_locks[tc->ident.proc_index];
    pthread_spin_lock(lock);
  }
  tc->blocked = 0;

  // insert in queue
  int proc_index = tc->ident.proc_index;
  runnable_tcs[proc_index][runnable_count[proc_index]++] = tc;

  // TODO: actually restart the target processor, if it was stopped
  if (!same_processor) {
    pthread_spin_unlock(lock);
    //announce_runnable_not_empty(proc_index);
  }
}

void yield(tc_t* yielding_tc) {
  LOG(VERBOSE, "yielding tc\n");
  //getcontext(&yielding_tc->context);
  //setcontext(&processor[yielding_tc->ident.proc_index].scheduler_context);
  swapcontext(&yielding_tc->context,
               &processor[yielding_tc->ident.proc_index].scheduler_context);
}

void block_tc_and_unlock(tc_t* tc, pthread_spinlock_t* lock) {
  // assumes tc is the current TC
  tc->blocked = 1;
  if (lock != NULL) pthread_spin_unlock(lock);
  // we sleep
  yield(tc);
  // ... and we wake up
}


int atomic_increment_next_tc(int proc_id) {
  int rez = -1;
  LOG(DEBUG, "atomic_increment_next_tc %d\n", proc_id);
  pthread_spin_lock((pthread_spinlock_t*)&allocate_tc_locks[proc_id]);
  int i;
  int start_tc = proc_id * NO_TCS_PER_PROC;
  for (i = start_tc; i < start_tc + NO_TCS_PER_PROC; ++i) {
    if (!tc_is_valid(i)) continue;  // skip TC's that haven't been created (due to wholes in the addr space)
    tc_t* tc = (tc_t*)TC_START_VM(i);
    //if (tc->blocked == -1) {
    if (tc->finished == 1) {
      rez = i;
      tc->blocked = 0;
      tc->finished = 0;
      break;
    }
  }
  pthread_spin_unlock((pthread_spinlock_t*)&allocate_tc_locks[proc_id]);
  return rez;
}


void rt_quit() {
  //TODO:
  //assert(0);
  exit(0);
}


void sighandler_foo(int sig, siginfo_t *si, void *ucontext);


pthread_mutex_t main_finished_mutex;
pthread_cond_t main_finished_cv;
volatile int main_finished;


void end_main() {
  pthread_mutex_lock(&main_finished_mutex);
  main_finished = 1;
  pthread_cond_signal(&main_finished_cv);
  pthread_mutex_unlock(&main_finished_mutex);
  _return_to_scheduler();
}

//extern fam_main_t fam_main;
extern fam_main_t _fam_t_main;

/*
  main function; sets up the runtime and creates fam_main, with one thread
 */
int main(int argc, char** argv) {

  struct sigaction sa;
  sa.sa_sigaction = sighandler_foo;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;

  if (sigaction(SIGSEGV, &sa, NULL) != 0)
  { perror("sigaction"); exit(1); }


  rt_init();

  fam_context_t* fam = allocate_fam_main(&_fam_t_main);
  printf("creating main\n");
  create_fam(fam);
  pthread_mutex_lock(&main_finished_mutex);
  while (!main_finished) {
    LOG(DEBUG, "SLEEPING\n");
    pthread_cond_wait(&main_finished_cv, &main_finished_mutex);
    LOG(DEBUG, "WOKE UP\n");
  }
  pthread_mutex_unlock(&main_finished_mutex);
  //sync_fam(fam);

  printf("USER:\t main thread done\n");
  //while(1) {}
  rt_quit();

  return 0;
}

void write_global(fam_context_t* ctx, int index, long val) {
  int i;
  // write the global to every TC that will run part of the family
  for (i = 0; i < ctx->no_ranges; ++i) {
    tc_ident_t id = ctx->ranges[i].dest;
    assert(id.node_index == cur_tc->ident.node_index);  // TODO
    tc_t* dest = (tc_t*)TC_START_VM_EXTENDED(id.node_index, id.tc_index);
    assert(dest == id.tc);  // TODO: if this assert proves to hold, remove the line above
    write_istruct(&(id.tc->globals[index]), val, &id);
  }
}

void sighandler_foo(int sig, siginfo_t *si, void *ucontext)
{
    char *page =
      (char*)((unsigned long)si->si_addr
          & ~((unsigned long)getpagesize() - 1L));
    printf("sig handler for %d, fault %p, page %p\n", sig, si->si_addr, page);

    exit(1);
    return;
}

