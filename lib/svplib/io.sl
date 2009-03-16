m4_include(svp/lib.slh)
m4_include(svp/div.slh)
m4_include(svp/io.slh)

static const char *digits = "0123456789abcdef";

sl_def(svp_io_putc, void,
       sl_glparm(char, c))
{
  __write1(sl_getp(c));
}
sl_enddef

sl_def(svp_io_puts, void,
       sl_glparm(const char *, gstr))
{
  const char *str = sl_getp(gstr);
  char c;
  while (likely(c = *str++))
    __write1(c);
}
sl_enddef

sl_def(svp_io_putf, void,
       sl_glfparm(double, gx),
       sl_glparm(unsigned, gprec),
       sl_glparm(unsigned, gbase))
{
  double x = sl_getp(gx);
  unsigned prec = sl_getp(gprec);
  const unsigned base = sl_getp(gbase);

  if (unlikely(x != x)) sl_proccall(svp_io_puts, sl_glarg(const char*, s, "nan"));
  else if (unlikely(x == 1e5000)) sl_proccall(svp_io_puts, sl_glarg(const char*, s, "+inf")); 
  else if (unlikely(x == -1e5000)) sl_proccall(svp_io_puts, sl_glarg(const char*, s, "-inf"));
  else {
      /* -- find exponent and normalize -- */
      int exp = 0;
      while (x >= base) { x /= base; ++exp; }
      while (x < 1.0) { x *= base; --exp; }

      /* -- print the mantissa -- */
      if (x < 0.) { __write1('-'); x = -x; }

      unsigned d = (unsigned)x;
      __write1(digits[d]);
      __write1('.');

      x = (x - d) * base;
      while(--prec) {
	d = (unsigned)x;
	__write1(digits[d]);
	x = (x - d) * base;
      }

      /* -- print the exponent -- */
      __write1('E');
      if (exp < 0) { __write1('-'); exp = -exp; }
      unsigned uexp = exp;
      if (uexp < base)
	__write1(digits[uexp]);
      else {
	unsigned rs = base;
	divmodu(uexp, rs);
	__write1(digits[rs]);
	__write1(digits[uexp]);
      }
    }
}
sl_enddef

sl_def(svp_io_putun, void,
       sl_glparm(unsigned long long, gn),
       sl_glparm(unsigned, gbase))
{
  unsigned long long x = sl_getp(gn);
  const unsigned base = sl_getp(gbase);
  if (x < base) __write1(digits[x]);
  else {
      unsigned long long root = 1;
      while (divu(x, root) >= base)
	root *= base;
      while (root) {
	unsigned long long rs = root;
	divmodu(x, rs);
	__write1(digits[rs]);
	rs = base;
	divmodu(root, rs);
	root = rs;
      }
    }
}
sl_enddef

sl_def(svp_io_putn, void,	
       sl_glparm(long long, gn),
       sl_glparm(unsigned, gbase))
{
  long long x = sl_getp(gn); 
  const unsigned base = sl_getp(gbase);
  if (!x) __write1('0');
  else {
    long long root;
    if (x < 0) {
      root = -1;
      __write1('-');
    } else root = 1;
    while (divs(x, root) >= base)
      root *= base;
    while (root) {
      long long rs = root;
      divmods(x, rs);
      __write1(digits[rs]);
      rs = (long long)base;
      divmods(root, rs);
      root = rs;
    }
  }
}
sl_enddef

sl_def(svp_io_printf, void,
       sl_glparm(const char*, gfmt),
       sl_glparm(svp_arg_slot*, gdata))
{
  const char *fmt;
  const svp_arg_slot *data;

  for (fmt = sl_getp(gfmt), 
	 data = sl_getp(gdata); *fmt; ++fmt) { 
    if (likely(*fmt != '%'))
      __write1(*fmt);
    else 
      switch(*++fmt) {
      case 'c': sl_proccall(svp_io_putc, 
			    sl_glarg(char, c, (*data++).c)); 
	break;
      case 's': sl_proccall(svp_io_puts, 
			    sl_glarg(const char*, p, (const char*)(*data++).ptr)); 
	break;
      case 'f': sl_proccall(svp_io_putf, 
			    sl_glarg(double, d, (*data++).d), 
			    sl_glarg(unsigned, prec, 7),
			    sl_glarg(unsigned, base, 10));
	break;
      case 'g': sl_proccall(svp_io_putf, 
			    sl_glarg(double, d, (*data++).d), 
			    sl_glarg(unsigned, prec, 15),
			    sl_glarg(unsigned, base, 10));
	break;
      case 'd': sl_proccall(svp_io_putn, 
			    sl_glarg(long long, n, (*data++).s),
			    sl_glarg(unsigned, base, 10));
	break;
      case 'u': sl_proccall(svp_io_putun, 
			    sl_glarg(unsigned long long, n, (*data++).u),
			    sl_glarg(unsigned, base, 10)); 
	break;
      case 'x': sl_proccall(svp_io_putun, 
			    sl_glarg(unsigned long long, n, (*data++).u),
			    sl_glarg(unsigned, base, 16)); 
	break;
      default: __write1('%'); __write1(*fmt); break;
      }
  }
}

sl_enddef


