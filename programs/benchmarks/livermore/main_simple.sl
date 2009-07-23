//first, define the method name of kernel
//to execute here.
m4_define([[ACTUALKERNEL]], [[kernel]]KERNEL)

sl_def(t_main, void)
{  
  sl_create(,,,,,,,ACTUALKERNEL);
  sl_sync();
}
sl_enddef

