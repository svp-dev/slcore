int e;
// Simple control flow checking.
sl_def(t_main, void)
{ 
    volatile int i;

    if (e) { 
        i = i + 1;
    }
}
sl_enddef
