#include "hashtable/hashtable.h"
#include "hashtable/hashtable_itr.h"

#ifndef GOL
#define GOL

#define TRUE  1
#define FALSE 0

#define STD_OUT 1
#define STD_ERR 2

#define DEBUG 0
#define INFO 0
#define DISPLAY 0

#define MODE_SEQ 0
#define MODE_PAR_1 1
#define MODE_PAR_2 2

#define DEFAULT_QUEUE_SIZE 10
#define OUTPUT_BUFFER_SIZE 512



/* The chunk size (units of computation in the grid). */
int chunk_size;
/* The block size (number of threads per core per threadfamily). */
int block_size;
/* The number of allocated cores. */
int core_count;
/* The mode the program operates in. */
int mode;

int cycle;
/* A chunk struct that carries the state of partition of the GOL universe. */
typedef struct chunk {
  /* coordinate in the global universe */
  short x, y;
  /* neighboring chunks, by cardinal directions */
  struct chunk *nn, *ne, *ee, *se, *ss, *sw, *ww, *nw;
  /* grid state at current and next timestep, swapped each interation */
  char *curr;
  char *next; 
  /* active/inactive flag */
  char active_flag;
} chunk;

/* A struct that holds a pointer to an array of chunk pointers. */
typedef struct chunk_queue {
  size_t size;
  chunk **chunks;
  size_t elements;
} chunk_queue;

/* A struct that holds a pointer to an array of packed coordinates. */
typedef struct request_queue {
  unsigned int *coordinates;
  size_t size;
  size_t elements;
} request_queue;

/* A key struct that consists of two short coordinates. */
typedef struct key
{
  short x;
  short y;
} key;

/* The chunk queue */
chunk_queue* b_queue;

/* The request queue */
request_queue* r_queue;

/**
 * Sets up the queues and hashtable, prepare graphical output, read initial 
 * conditions and start simulation.
 */
 
void init();
//starting GOL
sl_decl(gol,void,sl_shparm(int,breaked));
//proceesing request
sl_decl(process_requests,void,sl_shparm(int,stat));

//sl_decl(read_request, void, sl_shparm(unsigned int, coordinate));
sl_decl(queue_request, void, sl_glparm(unsigned int, coordinate));

unsigned int pack_coordinates(short x, short y);

/**
 * Traverses the hashtable.
 */
//void traverse();

sl_decl(traverse,void,sl_shparm(int,state), sl_shparm(struct hashtable_itr*,itr));

/**
 * Processes all queued requests.
 */
//void process_requests();

/**
 * Extracts a key containing two short coordinates from a single unsigned int.
 */
void unpack_coordinates(unsigned int coordinates, short * x, short * y);

/**
 * Allocates a new chunk positioned at given coordinates.
 */
chunk* allocate_chunk(short x, short y);

/**
 * Displays the given chunk. 
 */
void display_(chunk* b);
#if defined(DISPLAY) && DISPLAY == 1
#define display(b) display_(b)
#else
#define display(b) ((void)0)
#endif


/**
 * Creates a chunk at given coordinates.
 */
void create(short x, short y);

/**
 * Allocates a two-dimensional array serving as the subgrid of a chunk.
 */
char** allocate_subgrid();

/**
 * Adds a chunk to the chunk queue.
 */
void queue_chunk(chunk *b);

/**
 * Outputs a formatted info message.
 */
void info_print_(const char *format, ...);
#if defined(INFO) && INFO == 1
#define info_print(fmt, ...) info_print_(fmt, ## __VA_ARGS__)
#else
#define info_print(Fmt, ...) ((void)0)
#endif

/**
 * Outputs a formatted debug message.
 */
void debug_print_(const char *format, ...);
#if defined(DEBUG) && DEBUG == 1
#define debug_print(fmt, ...) debug_print_(fmt, ## __VA_ARGS__)
#else
#define debug_print(Fmt, ...) ((void)0)
#endif

/**
 * Outputs a formatted error message.
 */
void error_print(const char *format, ...);

/**
 * Outputs a formatted error message and dies.
 */
void die(const char * format, ...);

#define mlikely(x)       __builtin_expect((x),1)
#define munlikely(x)     __builtin_expect((x),0)


#endif

