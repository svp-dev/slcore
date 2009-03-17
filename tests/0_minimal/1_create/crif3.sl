int e;

sl_def(foo, void) {} sl_enddef

sl_def(t_main, void)
{
    if (e == 0) {
        sl_create(,,,,,,, foo);
        sl_sync();
    }
}
sl_enddef
