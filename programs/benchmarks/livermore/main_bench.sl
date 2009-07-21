m4_include(svp/assert.slh)
m4_include(fibre.slh)

//first, define the method name of kernel
//to execute here.
m4_define([[ACTUALKERNEL]], [[kernel]]KERNEL)

sl_def(t_main, void)
{
	// first Fibre item is iteration count
	svp_assert(fibre_tag(0) == 0 && fibre_rank(0) == 0);
	unsigned loops = *(unsigned long*)fibre_data(0);

	// 2nd Fibre item is problem size
	svp_assert(fibre_tag(1) == 0 && fibre_rank(1) == 0);
	unsigned psize = *(unsigned long*)fibre_data(1);

	//set the required problem size
	//if the value is 0, then use default PSIZE
	//also make sure that the PSIZE is less that the
	//maximum size of the data arrays
	
	if (psize > 0) {
	  svp_assert(psize < LARGEST);
	  inner[KERNEL] = psize;
	}
	
	while (loops--)
	{
	  sl_create(,,,,,,,ACTUALKERNEL);
	  sl_sync();
	}
}
sl_enddef

