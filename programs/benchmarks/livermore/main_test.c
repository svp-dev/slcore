#include <svp/iomacros.h>
#include <svp/perf.h>
#include <svp/slr.h>

slr_decl(slr_var(unsigned, L, "number of iterations (default 1)"),
	 slr_var(unsigned, N, "livermore problem size (leave empty or 0 for default)"),
	 slr_var(int, B, "inner create block size (leave empty or -1 for default)"));

#define MAX_LOOPS 10000
int64_t cycle_counts[MAX_LOOPS];

sl_def(t_main, void)
{
  //outer loop counter
  unsigned i, loops = 1;
  //cycle counter
  uint64_t cycles;
  uint64_t totalcycles = 0;
  
  //set the required problem size
  // if the value is <=0 then use default PSIZE
  //(assumes and unspecified value is set to zero)
  //also make sure that the PSIZE is less that the
  //maximum size of the data arrays
  if (slr_len(N) && *slr_get(N))
    if (*slr_get(N) < LARGEST)
      inner[KERNEL] = *slr_get(N);

  //set block size
  if (slr_len(B))
    if (*slr_get(B) >= 0)
      blocksize[KERNEL] = *slr_get(B);
  
  //number of loops
  if (slr_len(L) && *slr_get(L) < MAX_LOOPS) 
    loops = *slr_get(L);

  puts("--------------------------\n");
  puts("   LIVERMORE LOOPS:  \n");
  puts("   *** ACTUALKERNEL *** \n\n"); 
  puts("Inner Loops: "); putu(inner[KERNEL]);
  puts("\nOuter Loops: ");
  putu(loops);
  puts("\nBlock Size: ");
  putu(blocksize[KERNEL]);
  puts("\nComputing...\n");
  
  //this main loop can be controlled with the array 
  //'outer' above to repeat a particular benchmark
  //several times, perhaps in order to form
  //a more reliable average.
  
  //now spawn the kernel
  for (i = 0; i < loops; ++i)
    {
      // get current cycle counter
      cycles = get_cycles();
      //create the singleton family -- call the kernel
      sl_create(,,,,,,,ACTUALKERNEL);
      sl_sync();
      //now calculate the cycles used just for the kernel
      cycle_counts[i] = get_cycles() - cycles;
      totalcycles += cycle_counts[i];
    }

  puts("\nCycles in each iteration:");
  uint64_t maxcount = 0, mincount = UINT64_MAX;
  for (i = 0; i < loops; ++i) {
    putc('\n'); putu(cycle_counts[i]);
    if (mincount > cycle_counts[i]) mincount = cycle_counts[i];
    if (maxcount < cycle_counts[i]) maxcount = cycle_counts[i];
  }
  puts("\n\nMax cycles: "); putu(maxcount);
  puts("\nMin cycles: "); putu(mincount);
  puts("\nAvg cycles: ");
  double avg = totalcycles / (double)loops;
  putf(avg, 7); putc('\n');
}
sl_enddef

