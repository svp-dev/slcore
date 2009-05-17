sl_def(innerk3, void,
        sl_shfparm(double,ql))
{
	sl_setp(ql, sl_getp(ql) + 1.0l);
}
sl_enddef

sl_def(t_main, void)
{
	sl_create(,, ,2,,,,innerk3,
		     sl_shfarg(double, qql, 0.0l));	
	sl_sync();
}
sl_enddef
