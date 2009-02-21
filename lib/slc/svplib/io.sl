#include <libutc.h>
m4_include(svp/lib.slh)
m4_include(svp/div.slh)
m4_include(svp/io.slh)

#include "primitives.h"

static const char *digits = "0123456789abcdef";

ut_def(svp_io_putc, void,
       ut_glparm(char, c))
{
  __write1(ut_getp(c));
}
ut_enddef

ut_def(svp_io_puts, void,
       ut_glparm(const char *, gstr))
{
  const char *str = ut_getp(gstr);
  char c;
  while (likely(c = *str++))
    __write1(c);
}
ut_enddef

ut_def(svp_io_putf, void,
       ut_glparm(double, gx),
       ut_glparm(unsigned, gprec),
       ut_glparm(unsigned, gbase))
{
  double x = ut_getp(gx);
  unsigned prec = ut_getp(gprec);
  const unsigned base = ut_getp(gbase);

  if (unlikely(x != x)) ut_proccall(svp_io_puts, ut_glarg(const char*, s, "nan"));
  else if (unlikely(x == 1e5000)) ut_proccall(svp_io_puts, ut_glarg(const char*, s, "+inf")); 
  else if (unlikely(x == -1e5000)) ut_proccall(svp_io_puts, ut_glarg(const char*, s, "-inf"));
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
ut_enddef

ut_def(svp_io_putun, void,
       ut_glparm(unsigned long long, gn),
       ut_glparm(unsigned, gbase))
{
  unsigned long long x = ut_getp(gn);
  const unsigned base = ut_getp(gbase);
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
ut_enddef

ut_def(svp_io_putn, void,	
       ut_glparm(long long, gn),
       ut_glparm(unsigned, gbase))
{
  long long x = ut_getp(gn);
  const unsigned base = ut_getp(gbase);
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
ut_enddef

ut_def(svp_io_printf, void,
       ut_glparm(const char*, gfmt),
       ut_glparm(svp_arg_slot*, gdata))
{
  const char *fmt;
  const svp_arg_slot *data;

  for (fmt = ut_getp(gfmt), 
	 data = ut_getp(gdata); *fmt; ++fmt) { 
    if (likely(*fmt != '%'))
      __write1(*fmt);
    else 
      switch(*++fmt) {
      case 'c': ut_proccall(svp_io_putc, 
			    ut_glarg(char, c, (*data++).c)); 
	break;
      case 's': ut_proccall(svp_io_puts, 
			    ut_glarg(const char*, p, (const char*)(*data++).ptr)); 
	break;
      case 'f': ut_proccall(svp_io_putf, 
			    ut_glarg(double, d, (*data++).d), 
			    ut_glarg(unsigned, prec, 7),
			    ut_glarg(unsigned, base, 10));
	break;
      case 'g': ut_proccall(svp_io_putf, 
			    ut_glarg(double, d, (*data++).d), 
			    ut_glarg(unsigned, prec, 15),
			    ut_glarg(unsigned, base, 10));
	break;
      case 'd': ut_proccall(svp_io_putn, 
			    ut_glarg(long long, n, (*data++).s),
			    ut_glarg(unsigned, base, 10));
	break;
      case 'u': ut_proccall(svp_io_putun, 
			    ut_glarg(unsigned long long, n, (*data++).u),
			    ut_glarg(unsigned, base, 10)); 
	break;
      case 'x': ut_proccall(svp_io_putun, 
			    ut_glarg(unsigned long long, n, (*data++).u),
			    ut_glarg(unsigned, base, 16)); 
	break;
      }
  }
}

ut_enddef


