//
// slr.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_SLR_H
# define SLC_SVP_SLR_H

#include <stddef.h>

extern struct __slr_base_t { ptrdiff_t offset; size_t nitems; } *__slr_base;

m4_define([[__slr_offset]],  [[__slr_base[ [[$1]] ].offset]])
m4_define([[__slr_baseptr]], [[((char*)(void*)__slr_base)]])
m4_define([[__slr_dataptr]], [[((void*)(__slr_baseptr + __slr_offset([[$1]])))]])
m4_define([[__slr_len]],     [[__slr_base[ [[$1]] ].nitems]])

m4_define([[__slr_acnt]],0)
m4_define([[__slr_dcnt]],0)
m4_define([[__slr_vars]],[[]])

m4_define([[slr_var]],[[
  m4_define([[__slr_len_$2]],[[__slr_len(]]__slr_acnt[[)]])
  m4_define([[__slr_get_$2]],[[(([[$1]]*)__slr_dataptr(]]__slr_acnt[[))]])
  m4_define([[__slr_vars]],m4_quote(__slr_vars)[[\0slr_arg:]]__slr_acnt[[:[[$2]]:[[$1]]:]]m4_dnl
m4_if([[$3]],,[[(no description)]],[[sl_cdquote $3 sl_cdquote]]))
  m4_step([[__slr_acnt]])
]])

m4_define([[slr_decl]],[[m4_dnl
  m4_do($@) m4_dnl
  const char *__slr_decl_[[]]__slr_dcnt = sl_cdquote[[]]__slr_vars[[]]sl_cdquote m4_dnl
m4_define([[__slr_vars]],[[]]) m4_step([[__slr_dcnt]]) m4_dnl
]])

m4_define([[slr_get]],[[__slr_get_$1]])
m4_define([[slr_len]],[[__slr_len_$1]])

#endif // ! SLC_SVP_SLR_H
