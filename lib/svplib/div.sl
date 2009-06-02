//
// div.sl: this file is part of the slc project.
//
// Copyright (C) 2009 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
// $Id$
//

m4_include(svp/lib.slh)

// algorithm courtesy of Michael Hicks

m4_define([[DIVCODEU]],[[
  [[$1]] divisor = ([[$3]]);
  [[$1]] dividend = ([[$2]]);
  [[$1]] answer = 0;
  unsigned long long shiftcount = 1;

  if (unlikely(!divisor)) svp_abort();
  while (likely((divisor & (1ULL << ((sizeof([[$1]]) << 3) - 1))) == 0) ) {
    divisor <<= 1;
    ++shiftcount;
  }

  while (likely(shiftcount > 0)){
    answer <<= 1;
    if (divisor <= dividend){
      dividend -= divisor;
      ++answer;
    }
    divisor >>= 1;
    --shiftcount;
  }

  ([[$3]]) = answer;
  ([[$2]]) = dividend;
			 ]])

m4_define([[DIVCODES]],[[
  [[$1]] divisor = ([[$3]]);
  [[$1]] dividend = ([[$2]]);
  [[$1]] answer = 0;
  unsigned long long shiftcount = 1;
  long long dendsign = 1, invsign = -1;
  if (unlikely(!divisor)) svp_abort();
  if (divisor > 0) { invsign = -invsign;
    divisor = -divisor; }
  if (dividend > 0) { dendsign = -dendsign;
    invsign = -invsign; dividend = -dividend; }
  while ( likely((divisor & (1LL << ((sizeof([[$1]]) << 3) - 2))) != 0) ){
    divisor <<= 1;
    ++shiftcount;
  }
  while (likely(shiftcount > 0)){
    answer <<= 1;
    if (divisor >= dividend){
      dividend -= divisor;
      --answer;
    }
    divisor >>= 1;
    --shiftcount;
  }
  ([[$3]]) = answer * invsign;
  ([[$2]]) = dividend * dendsign;
			 ]])


m4_define([[DIVFUNCU]],[[
sl_def([[$1]],void,sl_shparm([[$2]],x),sl_shparm([[$2]],y))
  {
    [[$2]] x1 = sl_getp(x), y1 = sl_getp(y);
    DIVCODEU([[$2]], x1, y1);
    sl_setp(x, x1);
    sl_setp(y, y1);
  }
sl_enddef]])

m4_define([[DIVFUNCS]],[[
sl_def([[$1]],void,sl_shparm([[$2]],x),sl_shparm([[$2]],y))
  {
    [[$2]] x1 = sl_getp(x), y1 = sl_getp(y);
    DIVCODES([[$2]], x1, y1);
    sl_setp(x, x1);
    sl_setp(y, y1);
  }
sl_enddef]])

DIVFUNCU(__divmodqu, unsigned long long)
DIVFUNCU(__divmodlu, unsigned int)
DIVFUNCS(__divmodqs, signed long long)
DIVFUNCS(__divmodls, signed int)
