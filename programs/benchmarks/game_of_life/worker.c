#include <stdio.h>
#include <svp/delegate.h>
#include "worker.h"
#include "gol.h"

/**
 * ...
 */
char next_state(char active_neighbors, char this_state) {
  if (this_state) { // if the cell is alive...
    if (active_neighbors < 2 || active_neighbors > 3) { // ...and has less than 2 or more than 3 neighbors...
      return FALSE; //...it dies!
    } else { // if it has between two and three neighbors
      return TRUE; //...it stays alive!
    }
  } else { // if the cell is dead...
    if (active_neighbors == 3) { //...and has exactly three neighbors...
      return TRUE; //...it comes alive!
    }
  }
  // per default...
  return FALSE; //...all cells are inactive
}

/**
 * Gets the state of cell at x,y in chunk b.
 */
char getval(int x, int y, chunk* b) {

  char (*q)[chunk_size][chunk_size];

  if (b == NULL) {
    return 0;
  } else {
    q = (char(*)[chunk_size][chunk_size])b->curr;
    return (*q)[x][y];
  }
}

/**
 * Sets the active flag for chunk b.
 */
char activate(chunk *b) {

  if (b != NULL) {
    b->active_flag = TRUE;
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
 * File's a request for a new chunk at coordinates x and y into the request 
 * queue.
 */
char request(short x, short y) {

  create(x, y);
  return TRUE;
}

void calc(short x, short y, int iter, int index, chunk* b) {

  char (*curr)[chunk_size][chunk_size];
  curr = (char(*)[chunk_size][chunk_size])b->curr;

  char (*next)[chunk_size][chunk_size];
  next = (char(*)[chunk_size][chunk_size])b->next;

  char val;
  int s = chunk_size;
  chunk* nw = b->nw;
  chunk* nn = b->nn;
  chunk* ne = b->ne;
  chunk* ee = b->ee;
  chunk* se = b->se;
  chunk* ss = b->ss;
  chunk* sw = b->sw;
  chunk* ww = b->ww;

  if (munlikely(x > (s-1) || y > (s-1))) {
    error_print("Thread %d, Coordinates outside bounds: x=%d, y=%d\n", index, x, y);
    return;
  }

  if(munlikely(s == 0)) {
    error_print("Thread %d, Aborting calculation due to zero chunksize\n", index);
    return;
  }

  if (munlikely(s == 1)) { // special case NOT TESTED AT ALL (i.e. not used in regular setup)
    val = getval(0,0,nw)
        + getval(0,0,nn)
        + getval(0,0,ne)
        + getval(0,0,ee)
        + getval(0,0,se)
        + getval(0,0,ss)
        + getval(0,0,sw)
        + getval(0,0,ww);
    (*next)[0][0] = next_state(val, (*curr)[0][0]);
    if ((*next)[0][0] != (*curr)[0][0] || ((*curr)[0][0] != 0 && iter == 0)) {
      if (activate(nw) || request(b->x-1, b->y-1)){}
      if (activate(nn) || request(b->x, b->y-1)){}
      if (activate(ne) || request(b->x+1, b->y-1)){}
      if (activate(ee) || request(b->x+1, b->y)){}
      if (activate(se) || request(b->x+1, b->y+1)){}
      if (activate(ss) || request(b->x, b->y+1)){}
      if (activate(sw) || request(b->x-1, b->y+1)){}
      if (activate(ww) || request(b->x-1, b->y)){}
      activate(b);
    }
  } else { // common case
    if (y == 0) {
      if (x == 0) {
        debug_print("Thread %d: Calculating NW corner.\n", index);
        // outer part of field: corner cases
        // northwest corner (coordinates: [0][0])
        // always clockwise calculation
        val = getval(s-1,s-1,nw) //nw cell
            + getval(0,s-1,nn)   //nn cell
            + getval(1,s-1,nn)   //ne cell
            + (*curr)[1][0]            //ee cell
            + (*curr)[1][1]            //se cell
            + (*curr)[0][1]            //ss cell
            + getval(s-1,1,ww)   //sw cell
            + getval(s-1,0,ww);  //ww cell
        (*next)[0][0] = next_state(val, (*curr)[0][0]);
        if ((*next)[0][0] != (*curr)[0][0] || ((*curr)[0][0] != 0 && iter == 0)) {
          debug_print("Activating NW neighbour\n");
          if (activate(nw) || request(b->x-1, b->y-1)){}
          if (activate(nn) || request(b->x, b->y-1)){}
          if (activate(ww) || request(b->x-1, b->y)){}
          activate(b);
        }
      } else if (x == s-1) {
        debug_print("Thread %d: Calculating NE corner.\n", index);
        // northeast corner (coordinates: [s-1][0])
        val = getval(s-2,s-1,nn) //nw cell
            + getval(s-1,s-1,nn) //nn cell
            + getval(0,s-1,ne)   //ne cell
            + getval(0,0,ee)     //ee cell
            + getval(0,1,ee)     //se cell
            + (*curr)[s-1][1]          //ss cell
            + (*curr)[s-2][1]          //sw cell
            + (*curr)[s-2][0];         //ww cell

        debug_print("Thread %d: Calculating NE corner.\n", index);
        (*next)[s-1][0] = next_state(val, (*curr)[s-1][0]);
        if ((*next)[s-1][0] != (*curr)[s-1][0] || ((*curr)[s-1][0] != 0 && iter == 0)) {
          debug_print("Activating NE neighbour\n");
          if (activate(nn) || request(b->x, b->y-1)){}
          if (activate(ne) || request(b->x+1, b->y-1)){}
          if (activate(ee) || request(b->x+1, b->y)){}
          activate(b);
        }
      } else {
	// something along the north edge
        val = getval(x-1,s-1,nn) //nw cell
            + getval(x,s-1,nn)   //nn cell
            + getval(x+1,s-1,nn) //ne cell
            + (*curr)[x+1][0]          //ee cell
            + (*curr)[x+1][1]          //se cell
            + (*curr)[x][1]            //ss cell
            + (*curr)[x-1][1]          //sw cell
            + (*curr)[x-1][0];         //ww cell
        (*next)[x][0] = next_state(val, (*curr)[x][0]);
        if ((*next)[x][0] != (*curr)[x][0] || ((*curr)[x][0] != 0 && iter == 0)) {
          debug_print("Activating N neighbour\n");
          if (activate(nn) || request(b->x, b->y-1)){}
          activate(b);
        }
      }
    } else {
      if (y == s-1) {
        if (x == s-1) {
          debug_print("Thread %d: Calculating SE corner.\n", index);
          // southeast corner (coordinates: [s-1][s-1])
          val = (*curr)[s-2][s-2]        //nw cell
              + (*curr)[s-1][s-2]        //nn cell
              + getval(0,s-2,ee)   //ne cell
              + getval(0,s-1,ee)   //ee cell
              + getval(0,0,se)     //se cell
              + getval(s-1,0,ss)   //ss cell
              + getval(s-2,0,ss)   //sw cell
              + (*curr)[s-2][s-1];       //ww cell
          (*next)[s-1][s-1] = next_state(val, (*curr)[s-1][s-1]);
          if ((*next)[s-1][s-1] != (*curr)[s-1][s-1] || ((*curr)[s-1][s-1] != 0 && iter == 0)) {
            debug_print("Activating SE neighbour\n");
            if (activate(ee) || request(b->x+1, b->y)){}
            if (activate(se) || request(b->x+1, b->y+1)){}
            if (activate(ss) || request(b->x, b->y+1)){}
            activate(b);
	  }
	} else if (x == 0) {
	  debug_print("Thread %d: Calculating SW corner.\n", index);
    	  // southwest corner (coordinates: [0][s-1])
          val = getval(s-1,s-2,ww) //nw cell
              + (*curr)[0][s-2]          //nn cell
              + (*curr)[1][s-2]          //ne cell
              + (*curr)[1][s-1]          //ee cell
              + getval(1,0,ss)     //se cell
              + getval(0,0,ss)     //ss cell
              + getval(s-1,0,sw)   //sw cell
              + getval(s-1,s-1,ww);//ww cell
          (*next)[0][s-1] = next_state(val, (*curr)[0][s-1]);
          if ((*next)[0][s-1] != (*curr)[0][s-1] || ((*curr)[0][s-1] != 0 && iter == 0)) {
            debug_print("Activating SW neighbour\n");
            if (activate(ss) || request(b->x, b->y+1)){}
            if (activate(sw) || request(b->x-1, b->y+1)){}
            if (activate(ww) || request(b->x-1, b->y)){}
            activate(b);
	  }
	} else {
	  // southern edge case
          val = (*curr)[x-1][s-2]        //nw cell
              + (*curr)[x][s-2]          //nn cell
              + (*curr)[x+1][s-2]        //ne cell
              + (*curr)[x+1][s-1]        //ee cell
              + getval(x+1,0,ss)   //se cell
              + getval(x,0,ss)     //ss cell
              + getval(x-1,0,ss)   //sw cell
              + (*curr)[x-1][s-1];       //ww cell
          (*next)[x][s-1] = next_state(val, (*curr)[x][s-1]);
          if ((*next)[x][s-1] != (*curr)[x][s-1] || ((*curr)[x][s-1] != 0 && iter == 0)) {
            debug_print("Activating S neighbour\n");
            if (activate(ss) || request(b->x, b->y+1)){}
            activate(b);
          }
	}
      } else {
        if (x == s-1) {
	  // eastern edge case
          val = (*curr)[s-2][y-1]        //nw cell
              + (*curr)[s-1][y-1]        //nn cell
              + getval(0,y-1,ee)   //ne cell
              + getval(0,y,ee)     //ee cell
              + getval(0,y+1,ee)   //se cell
              + (*curr)[s-1][y+1]        //ss cell
              + (*curr)[s-2][y+1]        //sw cell
              + (*curr)[s-2][y];         //ww cell
          (*next)[s-1][y] = next_state(val, (*curr)[s-1][y]);
          if ((*next)[s-1][y] != (*curr)[s-1][y] || ((*curr)[s-1][y] != 0 && iter == 0)) {
            debug_print("Activating E neighbour\n");
            if (activate(ee) || request(b->x+1, b->y)){}
            activate(b);
          }
	} else if (x == 0) {
	  // western edge
          val = getval(s-1,y-1,ww) //nw cell
              + (*curr)[0][y-1]          //nn cell
              + (*curr)[1][y-1]          //ne cell
              + (*curr)[1][y]            //ee cell
              + (*curr)[1][y+1]          //se cell
              + (*curr)[0][y+1]          //ss cell
              + getval(s-1,y+1,ww) //sw cell
              + getval(s-1,y,ww);  //ww cell
          (*next)[0][y] = next_state(val, (*curr)[0][y]);
          if ((*next)[0][y] != (*curr)[0][y] || ((*curr)[0][y] != 0 && iter == 0)) {
            debug_print("Activating W neighbour\n");
            if (activate(ww) || request(b->x-1, b->y)){}
            activate(b);
          }
	} else {
	  // base case
          val = (*curr)[x-1][y-1] + (*curr)[x][y-1] + (*curr)[x+1][y-1]
              + (*curr)[x-1][y]   + (*curr)[x+1][y]
              + (*curr)[x-1][y+1] + (*curr)[x][y+1] + (*curr)[x+1][y+1];
          (*next)[x][y] = next_state(val, (*curr)[x][y]);
          if ((*next)[x][y] != (*curr)[x][y] || ((*curr)[x][y] != 0 && iter == 0)) {
            b->active_flag = TRUE;
          }
	}
      }
    }
  }
}

sl_def(calc_parallel, void, sl_glparm(int, iter),
			    sl_glparm(int, index),
			    sl_glparm(chunk *, b)) {

  int index = sl_getp(index);
  sl_index(i);

  short x = i / chunk_size;
  short y = i % chunk_size;

  calc(x, y, sl_getp(iter), index, sl_getp(b));

} sl_enddef


/* Registers the SL run function */
/*sl_def(do_run, void, sl_glparm(int, iteration))
{
  // Get the index for the block pointer;
  sl_index(index);
  int iter = sl_getp(iteration);
  int s = chunk_size;
  char (*curr)[chunk_size][chunk_size];
  char (*next)[chunk_size][chunk_size];

  debug_print("Thread %d: Starting iteration %d\n", index, iter);

  if(munlikely(index >= (int)b_queue->elements))
  {
    error_print("Thread %d: Can't read chunk (not enough chunks in queue)\n", 
      index);
    sl_end_thread;
  }

  debug_print("Thread %d: Reading chunk info\n", index);
  debug_print("Thread %d: chunk size:%d.\n", index, s);
  chunk* b = b_queue->chunks[index];


  debug_print("Thread %d: chunk size:%d.\n", index, s);

  if (mlikely(mode == MODE_PAR_2)) {
    sl_create( , , 0, s*s, , block_size, , calc_parallel, sl_glarg(int, iter, iter),
		  				sl_glarg(int, index, index),
  						sl_glarg(chunk *, b, b));

    sl_sync();

    debug_print("Synced Cells for worker\n\n");
  } else {
    for (int x = 0; x < s; x++) {
      for (int y = 0; y < s; y++) {
        calc(x, y, iter, index, b);
      }
    }
  }


  // Do not set new state for the first iteration
  if(munlikely(iter == 0)) {
    curr = (char(*)[chunk_size][chunk_size])b->curr;
    next = (char(*)[chunk_size][chunk_size])b->next;

    for (int x = 0; x < s; x++)
      for (int y = 0; y < s; y++)
         (*next)[x][y] = (*curr)[x][y];

  }
}
sl_enddef

/*
sl_def(run, void, sl_glparm(int, iteration), sl_glparm(int, N))
{
    debug_print("Going to spawn %d threads\n", sl_getp(N));
    sl_create(,,,sl_getp(N),,,, do_run, sl_glarg(int,iteration, sl_getp(iteration)));
    sl_sync();
}
sl_enddef*/
sl_def(run, void, sl_glparm(int, iteration),sl_shparm(int,sta))
{
  // Get the index for the block pointer;
  sl_index(index);
  int iter = sl_getp(iteration);
  int flag = sl_getp(sta);
  if(flag==0)
  {
	  int s = chunk_size;
	  char (*curr)[chunk_size][chunk_size];
	  char (*next)[chunk_size][chunk_size];
	  debug_print("Thread %d: Starting iteration %d\n", index, iter);
	  debug_print("Thread %d: Reading chunk info\n", index);
      debug_print("Thread %d: chunk size:%d.\n", index, s);
  
	  chunk* b = b_queue->chunks[index];
	  debug_print("Thread %d: chunk size:%d.\n", index, s);
	  if (mode == MODE_PAR_2)
	  {
		  sl_create( , , 0, s*s, , block_size, , calc_parallel, sl_glarg(int, iter, iter),
					sl_glarg(int, index, index),sl_glarg(chunk *, b, b));
		  sl_sync();
		  debug_print("Synced Cells for worker\n");
	  } 
	  else
	  {
		for (int x = 0; x < s; x++) 
			for (int y = 0; y < s; y++) 
				calc(x, y, iter, index, b);
	   }


  // Do not set new state for the first iteration
	  if(iter == 0)
	  {
			curr = (char(*)[chunk_size][chunk_size])b->curr;
			next = (char(*)[chunk_size][chunk_size])b->next;
			for (int x = 0; x < s; x++)
				for (int y = 0; y < s; y++)
					(*next)[x][y] = (*curr)[x][y];

	  } 
	  if(index + 1 == (int)b_queue->elements)
		flag = 1;
		
	  sl_setp(sta,flag);
	  
	  if(flag == 1)
		sl_break;
	 
  }
  else
  {
	debug_print("Request processing Thread %d: Can't read chunk (no more chunks in the queue)\n", index);
	sl_setp(sta,flag);
  }
  	

}
sl_enddef
