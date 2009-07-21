m4_ifndef([[KERNEL]],[[m4_error([[KERNEL must be defined here.]])]])
	
m4_include(ll.slh)
m4_include([[k]]KERNEL.sl)
m4_include(main_bench.sl)


