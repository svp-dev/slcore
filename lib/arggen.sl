m4_include(svp/iomacros.slh)
m4_include(cstddef.slh)

m4_include(data.c)

sl_def(t_main, void)
{
  write(&__sgr_data, sizeof(__sgr_data));
}
sl_enddef

