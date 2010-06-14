#ifndef RT_H
#define RT_H

#include <ucontext.h>
#include <pthread.h>
#include <signal.h>
#include <assert.h>
void exit(int);

struct tc_t;

struct tc_ident_t {
  int node_index;
  int proc_index;
  int tc_index;
  struct tc_t* tc;
};
typedef struct tc_ident_t tc_ident_t;


/* maximum number of shareds and globals (independenty, not
   in total) that a thread function can take */
#define MAX_ARGS_PER_FAM 32  


enum istruct_state {EMPTY = 0, WRITTEN, SUSPENDED};
typedef enum istruct_state istruct_state;

/*
 * Represents an I-structure; the decision has been to not mark any fields as volatile;
 * if needed when actually used, the whole struct should be declared as volatile. Sometimes
 * there's no need to do this (e.g. when both the reader and the writer are running on the same
 * TC, there's no asynchrony). 
 */
struct i_struct {
  enum istruct_state state;
  long data;
  pthread_spinlock_t lock;  // used in some cases to guarantee that 
                            // reading and suspending  or  writing and waking up the suspended TC
                            // are atomic operations
};
typedef struct i_struct i_struct;

struct heap_t {
  void* lowaddr;
  int size;
};
typedef struct heap_t heap_t;

struct thread_range_t {
  long index_start, index_end;  // inclusive
  tc_ident_t dest;
};

struct fam_context_t {
  int empty;  // 1 if the fam_context is available for reuse (nobody is going to sync on it)
  i_struct done;  // written on family termination
  //pthread_spinlock_t lock;  // protects .done
  tc_ident_t* blocked_tc;
  struct thread_range_t ranges[100];  //TODO: replace this with something else 
  int no_ranges;
  //i_struct shareds[MAX_ARGS_PER_FAM];  // shareds written by the last thread in fam
  long shareds[MAX_ARGS_PER_FAM];  // shareds written by the last thread in the fam. They don't need to
                                   // be i-structs since they will only be read after the sync
  //i_struct globals[MAX_ARGS_PER_FAM];  // globals share the storage (on each node that runs a family)
};
typedef struct fam_context_t fam_context_t;

struct tc_t {
  tc_ident_t ident;  // identity of this TC
  long index_start, index_stop;  // indexes are inclusive
  int finished;  // 1 if the TC is available for reuse
  int blocked;  // -1 means empty, 0 means running, 1 means blocked (so far on an istruct, but we might want
                // to extend this ?)

  ucontext_t context;  // contains stack in .uc_stack
  stack_t initial_thread_stack;  // the stack that will be reused every time when a new family is started
                //TODO: decide to what extent the .size member has any meaning. If it does, we need to trap
                // on stack auto-growth and update it.

  i_struct shareds[MAX_ARGS_PER_FAM];
  i_struct globals[MAX_ARGS_PER_FAM];
  fam_context_t* fam_context;  // family context of the thread of the family 
                               // currently occupying the TC
  tc_ident_t parent_ident;  // identifier of the TC where the parent is running; will
                            // have .node_index == -1 for families that don't have a parent
                            // (i.e. fam_main and continuation creates)
  tc_ident_t prev, next;  // identifiers of the TCs running the previous and next chunks of
                          // threads; for the first TC and the last TC, prev and next, respectively,
                          // will be the parent
  int is_last_tc;   // 1 if this is the TC where the last chunk of threads in a fam was allocated,
                    // 0 otherwise
  heap_t heap;
};//TCS[NO_TCS];
typedef struct tc_t tc_t;

/* data structure assigned to a processor */
struct processor_t {
  ucontext_t scheduler_context;
};
typedef struct processor_t processor_t;

#define NO_TCS_PER_PROC (1<<10)
#define MAX_PROCS_PER_NODE 16
#define INIT_TC_STACK_SIZE (1L<<13)
#define INIT_TC_HEAP_SIZE (1L<<20)

processor_t processor[MAX_PROCS_PER_NODE];
extern processor_t processor[MAX_PROCS_PER_NODE];

typedef void (*thread_func)();  // type of a thread function

extern __thread tc_t* cur_tc;  // pointer to the TC currently occupying the accessing processor



// represents an identifier of a concurrency node in the tree constructed by the mapping engine
struct mapping_node_t {
  //TODO
};

struct proc_assignment {
  //unsigned long no_threads;
  unsigned int load_percentage;
  unsigned int no_tcs;
  int node_index;
  int proc_index;
};
typedef struct proc_assignment proc_assignment;

/* Represent a mapping decision from the mapping engine */
struct mapping_decision {
  int should_inline;  // if this is 1, the other fields are irelevant; the
                      // family is supposed to be inlined in the parent
  unsigned int no_proc_assignments;
  struct proc_assignment proc_assignments[100];  // TODO: replace this with smth else
};
typedef struct mapping_decision mapping_decision;

fam_context_t* allocate_fam(
    thread_func func, 
    int num_shareds, int num_globals,
    long start_index, 
    long end_index,
    long step,
    struct mapping_node_t* parent_id, 
    const struct mapping_decision* mapping);

mapping_decision map_fam(
    thread_func func,
    long no_threads,
    //long start_index,
    //long end_index,
    struct mapping_node_t* parent_id);

tc_ident_t create_fam(fam_context_t* fc);

long sync_fam(fam_context_t* fc, /*long* shareds_dest,*/ int no_shareds, ...);
//TODO: release was removed, as it appears there's nothing for it to do. Recheck the
//decision.
//void release_fam(fam_context_t* fc);

/* Used by a thread func to get it's range of indexes */
static inline long _get_start_index() {
  return cur_tc->index_start;
}
static inline long _get_end_index() {
  return cur_tc->index_stop;
}
static inline fam_context_t* _get_fam_context() {
  return cur_tc->fam_context;
}
static inline const tc_ident_t* _get_parent_ident() {
  return &cur_tc->parent_ident;
}

static inline const tc_ident_t* _get_prev_ident() {
  return &cur_tc->prev;
}

static inline const tc_ident_t* _get_next_ident() {
  return &cur_tc->next;
}

static inline int _is_last_tc() {
  return cur_tc->is_last_tc;
}


static inline void _return_to_scheduler() {
  
  //swapcontext(&cur_tc->scratch_context, &processor[cur_tc->ident.proc_index].scheduler_context);
  //TODO: check that this call to setcontext is correct; the man says that the context used must be obtained
  //through getcontext() or makecontext(), which is not true in this case; it has been obtained by swapcontext()
  setcontext(&processor[cur_tc->ident.proc_index].scheduler_context);
}


void rt_init();
void rt_quit();


// called by fam_main at the end, instead of the regular thread function exit code
void end_main();

typedef void (fam_main_t)(void);  // the type of fam_main()



/* writer and reader are running in the same TC */
static inline void write_istruct_same_tc(
  i_struct* istructp, long val) {
  assert(istructp->state == WRITTEN);  //the istruct must have been written already for the current thread;
  istructp->data = val;
}
void write_istruct_same_proc(
    volatile i_struct* istructp, 
    long val, 
    tc_t* potentially_blocked_tc);
void write_istruct_different_proc(
    volatile i_struct* istructp,
    long val,
    tc_t* potentially_blocked_tc);
void write_istruct(volatile i_struct* istructp, 
                   long val, 
                   const tc_ident_t* reading_tc);


static inline long read_istruct_same_tc(i_struct* istruct) {
  assert(istruct->state == WRITTEN);
  //istruct->state = EMPTY;
  return istruct->data;
}
long read_istruct_different_tc(volatile i_struct* istruct, int same_proc);
long read_istruct(volatile i_struct* istructp, const tc_ident_t* writing_tc);

void write_global(fam_context_t* ctx, int index, long val);

//--------------------------------------------------
// /* read a shared from a child family 
//  * TODO: maybe optimize for when the read is done after sync. Then the istruct can
//  * be read directly 
//  */
// static inline long read_shared_from_child(
//     fam_context_t* child_fam, 
//     int shared_index) {
//   assert(cur_tc != NULL);  // this should only fail for the parent of fam_main,
//                            // but fam_main doesn't have any shareds
//   return read_istruct(&child_fam->shareds[shared_index],
//                       &child_fam->ranges[child_fam->no_ranges].dest);
// }
// 
//-------------------------------------------------- 

#endif
