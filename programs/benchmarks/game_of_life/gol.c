#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <svp/gfx.h>
#include <svp/perf.h>
#include <svp/assert.h>
#include <svp/fibre.h>
#include <svp/testoutput.h>
#include <svp/sep.h>
#include <svp/slr.h>
#include <svp/io.h>
#include <svp/delegate.h>
#include "gol.h"
#include "worker.h"


int xmin, ymin, xmax, ymax;

struct hashtable *table;
struct hashtable_itr *iterator;

slr_decl(slr_var(int, chunk), slr_var(int, cores), slr_var(int, block), slr_var(int, mode),slr_var(int, iter));

/* an exclusive place */
sl_place_t secretary_id;
sl_place_t worker_place_id;

/* hashtable marcro's */
DEFINE_HASHTABLE_INSERT(insert_some, struct key, struct chunk);
DEFINE_HASHTABLE_SEARCH(search_some, struct key, struct chunk);
DEFINE_HASHTABLE_REMOVE(remove_some, struct key, struct chunk);
DEFINE_HASHTABLE_ITERATOR_SEARCH(search_itr_some, struct key);

/* Registers the SL main function */
sl_def(t_main, void)
{

  init();
  sl_create(,worker_place_id,,,0,block_size,,gol,sl_sharg(int,breaked,0));
  sl_sync();

}
sl_enddef


/**
GOL
**/
sl_def(gol,void,sl_shparm(int,breaked))
{

	sl_index(index);
	int flag = sl_getp(breaked);
		
	if(flag==0)
	{	
		//info_print("GOL Thread %d :Processing %d blocks\n", index, b_queue->elements);
		//printf("GOL Thread %d :Processing %d blocks\n", index, b_queue->elements);
		//info_print("%d\n",iter);
		if( index + 1 == cycle)
			flag = 1;
		debug_print("Creating Worker family...\n");
		sl_create(,,,,0,block_size,,run,sl_glarg(int,iteration,index),sl_sharg(int,sta,0));
		debug_print("Waiting for sync...\n");
		sl_sync();
        
		debug_print("Workers finished, processing request queue...\n");
		sl_create(,,,,0,block_size,,process_requests,sl_sharg(int,stat,0));
		debug_print("Waiting for sync...\n");
		sl_sync();
  		
		debug_print("Request queue processing finished, traversing...\n");
		
		b_queue->elements = 0;
	
		sl_create(,,,,0,block_size,,traverse,sl_sharg(int,state,1),sl_sharg(struct hashtable_itr*,itr,hashtable_iterator(table)));
		debug_print("Waiting for sync...\n");
		sl_sync();			
		
		
		sl_setp(breaked,flag);
		
		if(flag == 1)
			sl_break;				
	}	
	else
	{
		//info_print("GOL Thread %d: BREAK family (Maximum iteration reached!)\n", index);
		//printf("GOL Thread %d: BREAK family (Maximum iteration reached!)\n", index);
		//info_print("\n");
		sl_setp(breaked,flag);
	}
		
	
}sl_enddef






/**
 * Packs two short coordinates into a single unsigned int.
 */
unsigned int pack_coordinates(short x, short y) 
{
  return ( x << (sizeof(short) * 8) | (y & (0 | (unsigned short) -1)));
}

/**
 * Extracts a key containing two short coordinates from a single unsigned int.
 */
void unpack_coordinates(unsigned int coordinates, short * x, 
     short * y) 
{
  *x = (short)((coordinates >> (sizeof(short) * 8)) & (0 | (unsigned short) -1));
  *y = (short)(coordinates & (0 | (unsigned short) -1));  
}

/**
 * Constructs a key from two short coordinates.
 */
key* key_from_coordinates(short x, short y) {
  key *k = malloc(sizeof(key));
  k->x = x;
  k->y = y;
  return k;
}

/**
 * Constructs a hash from given key.
 */
static unsigned int hashfromkey(void *ky)
{
  key *k = (key *)ky;
  return pack_coordinates(k->x, k->y);
}

/**
 * Tests whether two keys are equal.
 */
static int equalkeys(void *ky1, void *ky2) 
{
  key *k1 = (key*)ky1;
  key *k2 = (key*)ky2;

  return (k1->x == k2->x && k1->y == k2->y);
}

/**
 * Allocates a new chunk positioned at given coordinates.
 */
chunk* allocate_chunk(short x, short y)
{
  chunk* b = malloc(sizeof(chunk));
  char (*curr)[chunk_size][chunk_size];
  char (*next)[chunk_size][chunk_size];

  if (munlikely(b == NULL))
    die("Unable to allocate new chunk: out of memory.\n");

  b->x = x;
  b->y = y;
  b->active_flag = FALSE;
  b->nn = NULL;
  b->ne = NULL;
  b->nw = NULL;
  b->ww = NULL;
  b->ee = NULL;
  b->se = NULL;
  b->ss = NULL;
  b->sw = NULL;

  /* allocate subgrids filled with zeros */
  b->curr = malloc(chunk_size * chunk_size * sizeof(char));
  b->next = malloc(chunk_size * chunk_size * sizeof(char));

  if(munlikely(b->curr == NULL || b->next == NULL))
    die("Could not allocate subgrid: out of memory.\n");

  debug_print("Allocated subgrids");

  curr = (char(*)[chunk_size][chunk_size])b->curr;
  next = (char(*)[chunk_size][chunk_size])b->next;

  for(int i = 0; i < chunk_size; i++)
  {
    for(int j = 0; j < chunk_size; j++)
    {
      (*curr)[i][j] = (char)0;
      (*next)[i][j] = (char)0;
    }
  }

  debug_print("Initialized subgrids");

  /* allocation succeeded. */
  return b;
}

/**
 * Sets up the queues and hashtable, prepare graphical output, read initial 
 * conditions and start simulation.
 */
void init()
{

  unsigned long* arr;
  chunk* b;
  key* k;

  debug_print("[Begin init]\n");

  debug_print("Reading commandline arguments\n");

  chunk_size = 6;
  core_count = 4;
  block_size = 1;
  mode = MODE_PAR_2;
  cycle = 10;

  /* chunk size */
  if (slr_len(chunk) >= 1)
    chunk_size = slr_get(chunk)[0];

  /* core count */
  if (slr_len(cores) >= 1)
    core_count = slr_get(cores)[0];

  /* block size */
  if (slr_len(block) >= 1)
    block_size = slr_get(block)[0];

  /* mode */
  if (slr_len(mode) >= 1)
    mode = slr_get(mode)[0];
	
 /*iteration*/
  if(slr_len(iter) >= 1)
    cycle = slr_get(iter)[0];

  //info_print("Chunksize: %d, corecount: %d, blocksize: %d, mode: %d, iteration:%d\n", chunk_size, core_count, block_size, mode,cycle);
  printf("Chunksize: %d, corecount: %d, blocksize: %d, mode: %d, iteration:%d\n", chunk_size, core_count, block_size, mode,cycle);
  debug_print("Initializing queues\n");

  // NOTE: declare _AFTER_ reading chunk_size from file
  char (*q)[chunk_size][chunk_size];


  /* initialize chunk queue, start with default size */
  b_queue = malloc(sizeof(chunk_queue));
  b_queue->chunks = malloc(sizeof(chunk *) * DEFAULT_QUEUE_SIZE);
  b_queue->size = DEFAULT_QUEUE_SIZE;
  b_queue->elements = 0;

  /* initialize request queue, start with default size */
  r_queue = malloc(sizeof(request_queue));
  r_queue->coordinates = malloc(sizeof(int) * DEFAULT_QUEUE_SIZE);
  r_queue->size = DEFAULT_QUEUE_SIZE;
  r_queue->elements = 0;

  debug_print("Creating exclusive place\n");

  #if SVP_HAS_SEP
  sl_create(,root_sep->sep_place|1,,,,,, *root_sep->sep_alloc,
    sl_glarg(struct SEP*, , root_sep),
    sl_glarg(unsigned long, , SAL_DONTCARE),
    sl_sharg(struct placeinfo*, p, 0));
  sl_sync();
  if (sl_geta(p) == 0)
    die("Place allocation failed (sl_geta(p) == 0)\n");
  secretary_id = sl_geta(p)->pid | 1;
  #else
  secretary_id = PLACE_DEFAULT;
  #endif

  //info_print("Retrieved exclusive place ID: %x\n", secretary_id);
  printf("Retrieved secretary place ID: %x\n", secretary_id);

  if (mode == MODE_SEQ) 
    worker_place_id = PLACE_DEFAULT;
  else {
    #if SVP_HAS_SEP
    sl_create(,root_sep->sep_place,,,,,, *root_sep->sep_alloc,
      sl_glarg(struct SEP*, , root_sep),
      sl_glarg(unsigned long, , SAL_EXACT | core_count),
      sl_sharg(struct placeinfo*, q, 0));
    sl_sync();
    if (sl_geta(q) == 0)
      die("Place allocation failed (sl_geta(p) == 0)\n");
    worker_place_id = sl_geta(q)->pid;
    #else
    worker_place_id = PLACE_DEFAULT;
    #endif
  }
  //info_print("Retrieved worker place ID: %x\n", worker_place_id);
  printf("Retrieved worker place ID: %x\n", worker_place_id);

  debug_print("Initializing hashtable\n");

  /* init hashtable */
  table = create_hashtable(16, hashfromkey, equalkeys);
  if (table == NULL)
    die("Unable to create hashtable\n");

  debug_print("Reading initial state from file\n");

  /* allocate initial chunk */
  b = allocate_chunk(0, 0);
  q = (char(*)[chunk_size][chunk_size])b->curr;

  /* read values from file */
  debug_print("Coordinates:");
  for (int i=0; fibre_tag(i) != -1; i++)
  {
    /* fetch array of unsigned longs, containing at least two elements */
    if (fibre_shape(i)[0] >= 2 && fibre_tag(i) == 0) {
      arr = (unsigned long*)fibre_data(i);
      /* sanity check on the coordinates */
      svp_assert((int)arr[0] < chunk_size && (int)arr[1] < chunk_size);
      debug_print(" (%d,%d)", arr[0], arr[1]);
      (*q)[arr[0]][arr[1]] = TRUE;
    }
    else
      continue;
  }
 debug_print("\n");

  /* queue initial chunk */
  queue_chunk(b);

  /* add chunk to queue */
  k = malloc(sizeof(key));
  k->x = b->x;
  k->y = b->y;
  insert_some(table, k, b);

  /* init graphical subsystem */
  if (DISPLAY) {
    debug_print("Initializing graphical subsystem\n");  
    gfx_init();
    gfx_resize(11*chunk_size, 11*chunk_size);

    display(b);
  }
  debug_print("[End init]\n");

}

/**
 * Traverses the hashtable.
 */
sl_def(traverse,void,sl_shparm(int,state),sl_shparm(struct hashtable_itr *,itr))
{
	sl_index(index);
	int flag = sl_getp(state);
	struct hashtable_itr * ptr = sl_getp(itr);
	if(flag != 0)
	{
		struct chunk *chunk;	
	    char *tmp;
		chunk = hashtable_iterator_value(ptr);
		display(chunk);
		if(chunk->active_flag == TRUE)
		{
		/* set state inactive */
			chunk->active_flag = FALSE;
       /* swap pointers */
			tmp = chunk->curr;
			chunk->curr = chunk->next;
			chunk->next = tmp;
       /* administer corner coords */
			if(chunk->x < xmin)
				xmin = chunk->x;
			if(chunk->x > xmax)
				xmax = chunk->x;
			if(chunk->y < ymin)
				ymin = chunk->y;
			if(chunk->y > ymax)
				ymax = chunk->y;
				
			debug_print("Adding chunk to queue\n");
			queue_chunk(chunk);
		}
		debug_print("Ending hashtable traversal\n");
		
		flag = hashtable_iterator_advance(ptr);
		sl_setp(state,flag);
		sl_setp(itr,ptr);
		//info_print("Thread %d:family state %d \n", index,flag);
		if(flag == 0)
		  sl_break;
	}
	else
	{		
		debug_print("HashTable Traversing Thread %d: Can't read(no more entry in hashtable)!\n",index);
		sl_setp(state,flag);
		sl_setp(itr,ptr);
	}
	
}sl_enddef

/**
 * Creates a chunk at given coordinates.
 */
void create(short x, short y)
{
  unsigned int coord = pack_coordinates(x,y);
  debug_print("coords %u, (%d, %d)\n",coord, x, y);
  sl_create( , secretary_id , , , , , , queue_request,
  sl_glarg(unsigned int, coordinate, coord));
  //sl_seta(coordinate, coord);
  sl_sync();
}

/**
 * Adds a chunk to the chunk queue.
 */
void queue_chunk(chunk *b)
{
  /* if there is still a reasonable amount of space in the queue */
  if(munlikely(b_queue->elements < b_queue->size * 3 / 4))
  {
    /* put the chunk at address b in the chunks array */
    b_queue->chunks[b_queue->elements] = b;
    b_queue->elements++;
  }
  else
  {
    /* resize chunk array */
    chunk **bs = malloc(b_queue->size * 2 * sizeof(chunk *));
    size_t i = 0;
    for(i = 0; i < b_queue->elements; i++)
      bs[i] = b_queue->chunks[i];
    b_queue->chunks = bs;
    b_queue->size = b_queue->size * 2;

    /* still need to add the chunk to the expanded array */
    queue_chunk(b);
  }
}

/* Registers the SL queue_request function */
sl_def(queue_request, void, sl_glparm(unsigned int, coordinate)) {

  short x, y;
  unsigned int c = sl_getp(coordinate);

  unpack_coordinates(c, &x, &y);

  request_queue *q = r_queue;

  /* if there is still a reasonable amount of space in the queue */
  if(munlikely(q->elements < q->size * 3 / 4))
  {
    /* put the chunk at address b in the chunks array */
    q->coordinates[q->elements] = c;
    q->elements++;
  }
  else
  {
    /* resize chunk array */
    unsigned int *coords = malloc(q->size * 2 * sizeof(int));
    size_t i = 0;
    for(i = 0; i < q->elements; i++)
      coords[i] = q->coordinates[i];
    q->coordinates = coords;
    q->size = q->size * 2;

    /* still need to add the chunk to the expanded array */
    q->coordinates[q->elements] = c;
    q->elements++;
  }

} sl_enddef

/**
 * Checks whether the request queue is empty.
 */
char is_r_queue_empty(){
  if (r_queue->elements <= 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Retrieves another request from the request queue.
 */
unsigned int get_next_request(){
  request_queue *q = r_queue;
  unsigned int result;
  if(munlikely(is_r_queue_empty())) {
    die("Request Queue Underrun!");
  }
  result = q->coordinates[q->elements - 1];
  q->elements--;
  return result;
}

/**
 * Processes all queued requests.
 */
sl_def(process_requests,void,sl_shparm(int,stat)){
	short x, y;
	unsigned int packed_coordinates;
	chunk* b;
	sl_index(index);
	int flag = sl_getp(stat);
	if(flag==0)
	{   
		if(is_r_queue_empty() == 0)
		{
			
			packed_coordinates = get_next_request();
			unpack_coordinates(packed_coordinates, &x, &y);
			key *k = key_from_coordinates(x, y);
			b = search_some(table, k);
			if (b == NULL)
			{
				b = allocate_chunk(x, y);
				b->x = x;
				b->y = y;
				chunk *nw, *nn, *ne, *ee, *se, *ss, *sw, *ww;
				nw = search_some(table, key_from_coordinates(x-1, y-1));
				nn = search_some(table, key_from_coordinates(x, y-1));
				ne = search_some(table, key_from_coordinates(x+1, y-1));
				ee = search_some(table, key_from_coordinates(x+1, y));
				se = search_some(table, key_from_coordinates(x+1, y+1));
				ss = search_some(table, key_from_coordinates(x, y+1));
				sw = search_some(table, key_from_coordinates(x-1, y+1));
				ww = search_some(table, key_from_coordinates(x-1, y));
				if (nw != NULL) { b->nw = nw; nw->se = b; }
				if (nn != NULL) { b->nn = nn; nn->ss = b; }
				if (ne != NULL) { b->ne = ne; ne->sw = b; }
				if (ee != NULL) { b->ee = ee; ee->ww = b; }
				if (se != NULL) { b->se = se; se->nw = b; }
				if (ss != NULL) { b->ss = ss; ss->nn = b; }
				if (sw != NULL) { b->sw = sw; sw->ne = b; }
				if (ww != NULL) { b->ww = ww; ww->ee = b; }
				insert_some(table, k, b);
				b->active_flag = TRUE;
			}
		}
		
		flag = is_r_queue_empty();
		sl_setp(stat,flag);
		
		if(flag == 1)
			sl_break;			
    }
	else
	{
		debug_print("Thread %d : Can't read (no more request in the queue)!\n", index);
		sl_setp(stat,flag);
	}
	
}sl_enddef


/**
 * Displays the given chunk.
 */
void display_(chunk* b)
{
  /* find topleft coordinate of chunk for framebuffer */
  int x = ((int)b->x) * chunk_size + (5 * chunk_size);
  int y = ((int)b->y) * chunk_size + (5 * chunk_size);
  int index_x;
  int index_y;
  char (*q)[chunk_size][chunk_size];
  q = (char(*)[chunk_size][chunk_size])b->curr;

  for(index_x = 0; index_x < chunk_size; index_x++)
    for(index_y = 0; index_y < chunk_size; index_y++)
      if((*q)[index_x][index_y] == 0) {
        if (b->active_flag == FALSE) {
	  gfx_putpixel(x+index_x, y+index_y, 0x888888);
	} else {
          gfx_putpixel(x+index_x, y+index_y, 0xFFFFFF);
	}
      } else {
        gfx_putpixel(x+index_x, y+index_y, 0);
      }
}

/**
 * Outputs a formatted debug message.
 */
void info_print_(const char *format, ...)
{
  char s[OUTPUT_BUFFER_SIZE];
  va_list argptr;
  va_start(argptr,format);
  vsnprintf(s, OUTPUT_BUFFER_SIZE, format, argptr);
  va_end(argptr);

  if (secretary_id == 0)
    output_string(s, STD_OUT);
  else {
    sl_create( , secretary_id , , , , , , svp_io_puts,
    sl_glarg(const char *, gstr, s));
    sl_sync();
  }
}

/**
 * Outputs a formatted debug message.
 */
void debug_print_(const char *format, ...)
{
  char s[OUTPUT_BUFFER_SIZE];
  va_list argptr;
  va_start(argptr,format);
  vsnprintf(s, OUTPUT_BUFFER_SIZE, format, argptr);
  va_end(argptr);

  if (secretary_id == 0)
    output_string(s, STD_OUT);
  else {
    sl_create( , secretary_id , , , , , , svp_io_puts,
    sl_glarg(const char *, gstr, s));
    sl_sync();
  }
}

/**
 * Outputs a formatted error message.
 */
void error_print(const char *format, ...)
{
  char s[OUTPUT_BUFFER_SIZE];
  va_list argptr;
  va_start(argptr,format);
  vsnprintf(s, OUTPUT_BUFFER_SIZE, format, argptr);
  va_end(argptr);

  if (secretary_id == 0)
    output_string(s, STD_ERR);
  else {
    sl_create( , secretary_id , , , , , , svp_io_puts,
    sl_glarg(const char *, gstr, s));
    sl_sync();
  }
}

/**
 * Outputs a formatted error message and die.
 */
void die(const char * format, ...)
{
  char s[OUTPUT_BUFFER_SIZE];
  va_list argptr;
  va_start(argptr,format);
  vsnprintf(s, OUTPUT_BUFFER_SIZE, format, argptr);
  va_end(argptr);
  if (secretary_id == 0)
    output_string(s, STD_OUT);
  else {
    sl_create( , secretary_id , , , , , , svp_io_puts,
    sl_glarg(const char *, gstr, s));
    sl_sync();
  }
  output_string("\n...EXITING......\n", STD_ERR);
  abort();
}

