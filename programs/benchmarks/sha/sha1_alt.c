//
// sha1_alt.c: this file is part of the SL program suite.
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

#include <cstdint.h>
#include <cstddef.h>
#include <svp/slr.h>
#include <svp/assert.h>

#define ROL32(W, bits) (((W) << bits) | ((W) >> (32 - bits)))

sl_def(sha_main_inner, void,
       sl_glparm(const uint32_t*restrict, w),
       sl_shparm(unsigned long, a),
       sl_shparm(unsigned long, b),
       sl_shparm(unsigned long, c),
       sl_shparm(unsigned long, d),
       sl_shparm(unsigned long, e))
{
  sl_index(i);
  uint32_t d = sl_getp(d);
  uint32_t e = sl_getp(e);
  sl_setp(e, d);
  uint32_t c = sl_getp(c);
  sl_setp(d, c);
  uint32_t b = sl_getp(b);
  sl_setp(c, ROL32(b, 30));
  uint32_t a = sl_getp(a);
  sl_setp(b, a);
  uint32_t tmp = ROL32(a, 5) + e + sl_getp(w)[i];
  if (i < 20) {
    tmp += (b & c) | ((~b) & d);
    tmp += 0x5A827999L;
  } else if (i < 40) {
    tmp += (b ^ c ^ d);
    tmp += 0x6ED9EBA1L;
  } else if (i < 60) {
    tmp += (b & c) | (b & d) | (c & d);
    tmp += 0x8F1BBCDCL;
  } else {
    tmp += (b ^ c ^ d);
    tmp += 0xCA62C1D6L;
  }
  sl_setp(a, tmp);
}
sl_enddef

sl_def(buf_copy, void,
       sl_glparm(const uint32_t*restrict, src),
       sl_glparm(uint32_t*restrict, dst))
{
  sl_index(i);
  sl_getp(dst)[i] = sl_getp(src)[i];
}
sl_enddef

sl_def(sha_main_outer, void,
       sl_glparm(const uint32_t*restrict, input),
       sl_shparm(unsigned long, h0),
       sl_shparm(unsigned long, h1),
       sl_shparm(unsigned long, h2),
       sl_shparm(unsigned long, h3),
       sl_shparm(unsigned long, h4))
{
  sl_index(offset_base);
  int i;
  const uint32_t*restrict input = sl_getp(input) + offset_base;

  /* word extension: not easily made concurrent! */
  uint32_t w[80];
  sl_create(,PLACE_LOCAL,,16,,,, buf_copy,
	    sl_glarg(const uint32_t*restrict, src, input),
	    sl_glarg(uint32_t*restrict, dst, w));
  sl_sync();
  //  for (i = 0; i < 16; ++i) w[i] = input[i];
  for (i = 16; i < 80; ++i) {
    uint32_t x = w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16];
    w[i] = ROL32(x, 1);
  }

  sl_create(,,,80,,,, sha_main_inner,
	    sl_glarg(const uint32_t*restrict, wg, w),
	    sl_sharg(unsigned long, a),
	    sl_sharg(unsigned long, b),
	    sl_sharg(unsigned long, c),
	    sl_sharg(unsigned long, d),
	    sl_sharg(unsigned long, e));
  sl_seta(a, sl_getp(h0));
  sl_seta(b, sl_getp(h1));
  sl_seta(c, sl_getp(h2));
  sl_seta(d, sl_getp(h3));
  sl_seta(e, sl_getp(h4));
  sl_sync();
  sl_setp(h0, sl_getp(h0) + sl_geta(a));
  sl_setp(h1, sl_getp(h1) + sl_geta(b));
  sl_setp(h2, sl_getp(h2) + sl_geta(c));
  sl_setp(h3, sl_getp(h3) + sl_geta(d));
  sl_setp(h4, sl_getp(h4) + sl_geta(e));
}
sl_enddef

sl_def(sha1_kernel, void,
       sl_glparm(uint32_t*restrict, out),
       sl_glparm(const uint32_t*restrict, w),
       sl_glparm(size_t, len_words))
{
  sl_create(,,,sl_getp(len_words), 16,,, sha_main_outer,
	    sl_glarg(const uint32_t*restrict, wg, sl_getp(w)),
	    sl_sharg(unsigned long, h0),
	    sl_sharg(unsigned long, h1),
	    sl_sharg(unsigned long, h2),
	    sl_sharg(unsigned long, h3),
	    sl_sharg(unsigned long, h4));
  sl_seta(h0, 0x67452301L);
  sl_seta(h1, 0xEFCDAB89L);
  sl_seta(h2, 0x98BADCFEL);
  sl_seta(h3, 0x10325476L);
  sl_seta(h4, 0xC3D2E1F0L);
  sl_sync();
  uint32_t*restrict out = sl_getp(out);
  out[0] = sl_geta(h0);
  out[1] = sl_geta(h1);
  out[2] = sl_geta(h2);
  out[3] = sl_geta(h3);
  out[4] = sl_geta(h4);
}
sl_enddef

#define MAX_CHUNKS 10000

uint32_t data[16*MAX_CHUNKS];
uint32_t output[5];

slr_decl(slr_var(unsigned, N, "problem size (512-bit chunks)"));

// SLT_RUN: N=10
// XIGNORE: ppp:R

sl_def(t_main, void)
{
  unsigned sz = slr_get(N)[0];
  svp_assert(sz < MAX_CHUNKS);
  sl_create(,,,,,,, sha1_kernel,
	    sl_glarg(uint32_t*restrict, gout, output),
	    sl_glarg(const uint32_t*restrict, ginput, data),
	    sl_glarg(size_t, len, sz * 16));
  sl_sync();
}
sl_enddef
