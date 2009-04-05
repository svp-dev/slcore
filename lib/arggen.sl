m4_include(svp/iomacros.slh)
m4_include(cstddef.slh)

m4_include(data.c)

sl_def(t_main, void)
{
  write(&__slr_data, sizeof(__slr_data));
}
sl_enddef

