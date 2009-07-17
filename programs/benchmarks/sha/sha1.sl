
m4_include(cstdint.slh)
m4_include(cstddef.slh)
m4_include(slr.slh)
m4_include(svp/assert.slh)

#define ROL32(W, bits) (((W) << bits) | ((W) >> (32 - bits)))

sl_def(sha_main_0_19, void,
       sl_glparm(const uint32_t*restrict, w),
       sl_shparm(unsigned long, a),
       sl_shparm(unsigned long, b),
       sl_shparm(unsigned long, c),
       sl_shparm(unsigned long, d),
       sl_shparm(unsigned long, e))
{
  sl_index(i);
  uint32_t w = sl_getp(w)[i];
  uint32_t f = (sl_getp(b) & sl_getp(c)) | ((~sl_getp(b)) & sl_getp(d));
  uint32_t k = 0x5A827999L;
  uint32_t a = sl_getp(a);
  uint32_t tmp = ROL32(a, 5) + f + sl_getp(e) + k + w;
  sl_setp(e, sl_getp(d));
  sl_setp(d, sl_getp(c));
  uint32_t b = sl_getp(b);
  sl_setp(c, ROL32(b, 30));
  sl_setp(b, a);
  sl_setp(a, tmp);
}
sl_enddef

sl_def(sha_main_20_39, void,
       sl_glparm(const uint32_t*restrict, w),
       sl_shparm(unsigned long, a),
       sl_shparm(unsigned long, b),
       sl_shparm(unsigned long, c),
       sl_shparm(unsigned long, d),
       sl_shparm(unsigned long, e))
{
  sl_index(i);
  uint32_t w = sl_getp(w)[i];
  uint32_t f = (sl_getp(b) ^ sl_getp(c) ^ sl_getp(d));
  uint32_t k = 0x6ED9EBA1L;
  uint32_t a = sl_getp(a);
  uint32_t tmp = ROL32(a, 5) + f + sl_getp(e) + k + w;
  sl_setp(e, sl_getp(d));
  sl_setp(d, sl_getp(c));
  uint32_t b = sl_getp(b);
  sl_setp(c, ROL32(b, 30));
  sl_setp(b, a);
  sl_setp(a, tmp);
}
sl_enddef

sl_def(sha_main_40_59, void,
       sl_glparm(const uint32_t*restrict, w),
       sl_shparm(unsigned long, a),
       sl_shparm(unsigned long, b),
       sl_shparm(unsigned long, c),
       sl_shparm(unsigned long, d),
       sl_shparm(unsigned long, e))
{
  sl_index(i);
  uint32_t w = sl_getp(w)[i];
  uint32_t f = (sl_getp(b) & sl_getp(c)) | (sl_getp(b) & sl_getp(d)) | (sl_getp(c) & sl_getp(d));
  uint32_t k = 0x8F1BBCDCL;
  uint32_t a = sl_getp(a);
  uint32_t tmp = ROL32(a, 5) + f + sl_getp(e) + k + w;
  sl_setp(e, sl_getp(d));
  sl_setp(d, sl_getp(c));
  uint32_t b = sl_getp(b);
  sl_setp(c, ROL32(b, 30));
  sl_setp(b, a);
  sl_setp(a, tmp);
}
sl_enddef

sl_def(sha_main_60_79, void,
       sl_glparm(const uint32_t*restrict, w),
       sl_shparm(unsigned long, a),
       sl_shparm(unsigned long, b),
       sl_shparm(unsigned long, c),
       sl_shparm(unsigned long, d),
       sl_shparm(unsigned long, e))
{
  sl_index(i);
  uint32_t w = sl_getp(w)[i];
  uint32_t f = (sl_getp(b) ^ sl_getp(c) ^ sl_getp(d));
  uint32_t k = 0xCA62C1D6L;
  uint32_t a = sl_getp(a);
  uint32_t tmp = ROL32(a, 5) + f + sl_getp(e) + k + w;
  sl_setp(e, sl_getp(d));
  sl_setp(d, sl_getp(c));
  uint32_t b = sl_getp(b);
  sl_setp(c, ROL32(b, 30));
  sl_setp(b, a);
  sl_setp(a, tmp);
}
sl_enddef

sl_def(sha_main_inner, void,
       sl_glparm(const uint32_t*restrict, w),
       sl_shparm(unsigned long, a),
       sl_shparm(unsigned long, b),
       sl_shparm(unsigned long, c),
       sl_shparm(unsigned long, d),
       sl_shparm(unsigned long, e))
{
  sl_index(i);
  switch(i) {
  case 0:
    {
      sl_create(,, 0, 20, ,,, sha_main_0_19,
		sl_glarg(const uint32_t*restrict, wg, sl_getp(w)),
		sl_sharg(unsigned long, sa),
		sl_sharg(unsigned long, sb),
		sl_sharg(unsigned long, sc),
		sl_sharg(unsigned long, sd),
		sl_sharg(unsigned long, se));
      sl_seta(sa, sl_getp(a));
      sl_seta(sb, sl_getp(b));
      sl_seta(sc, sl_getp(c));
      sl_seta(sd, sl_getp(d));
      sl_seta(se, sl_getp(e));
      sl_sync();
      sl_setp(a, sl_geta(sa));
      sl_setp(b, sl_geta(sb));
      sl_setp(c, sl_geta(sc));
      sl_setp(d, sl_geta(sd));
      sl_setp(e, sl_geta(se));
    }
    break;
  case 1:
    {
      sl_create(,, 20, 40, ,,, sha_main_20_39,
		sl_glarg(const uint32_t*restrict, wg, sl_getp(w)),
		sl_sharg(unsigned long, sa),
		sl_sharg(unsigned long, sb),
		sl_sharg(unsigned long, sc),
		sl_sharg(unsigned long, sd),
		sl_sharg(unsigned long, se));
      sl_seta(sa, sl_getp(a));
      sl_seta(sb, sl_getp(b));
      sl_seta(sc, sl_getp(c));
      sl_seta(sd, sl_getp(d));
      sl_seta(se, sl_getp(e));
      sl_sync();
      sl_setp(a, sl_geta(sa));
      sl_setp(b, sl_geta(sb));
      sl_setp(c, sl_geta(sc));
      sl_setp(d, sl_geta(sd));
      sl_setp(e, sl_geta(se));
    }
    break;
  case 2:
    {
      sl_create(,, 40, 60, ,,, sha_main_40_59,
		sl_glarg(const uint32_t*restrict, wg, sl_getp(w)),
		sl_sharg(unsigned long, sa),
		sl_sharg(unsigned long, sb),
		sl_sharg(unsigned long, sc),
		sl_sharg(unsigned long, sd),
		sl_sharg(unsigned long, se));
      sl_seta(sa, sl_getp(a));
      sl_seta(sb, sl_getp(b));
      sl_seta(sc, sl_getp(c));
      sl_seta(sd, sl_getp(d));
      sl_seta(se, sl_getp(e));
      sl_sync();
      sl_setp(a, sl_geta(sa));
      sl_setp(b, sl_geta(sb));
      sl_setp(c, sl_geta(sc));
      sl_setp(d, sl_geta(sd));
      sl_setp(e, sl_geta(se));
    }
    break;
  case 3:
    {
      sl_create(,, 60, 80, ,,, sha_main_60_79,
		sl_glarg(const uint32_t*restrict, wg, sl_getp(w)),
		sl_sharg(unsigned long, sa),
		sl_sharg(unsigned long, sb),
		sl_sharg(unsigned long, sc),
		sl_sharg(unsigned long, sd),
		sl_sharg(unsigned long, se));
      sl_seta(sa, sl_getp(a));
      sl_seta(sb, sl_getp(b));
      sl_seta(sc, sl_getp(c));
      sl_seta(sd, sl_getp(d));
      sl_seta(se, sl_getp(e));
      sl_sync();
      sl_setp(a, sl_geta(sa));
      sl_setp(b, sl_geta(sb));
      sl_setp(c, sl_geta(sc));
      sl_setp(d, sl_geta(sd));
      sl_setp(e, sl_geta(se));
    }
    break;
  }
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
  sl_create(,,,16,,,, buf_copy, 
	    sl_glarg(const uint32_t*restrict, src, input), 
	    sl_glarg(uint32_t*restrict, dst, w));
  sl_sync();
  for (i = 16; i < 80; ++i) {
    uint32_t x = w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16];
    w[i] = x << 1 | x >> (32-1);
  }   

  sl_create(,,,4,,,, sha_main_inner,
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
  sl_create(,,,sl_getp(len_words), 16, 2,, sha_main_outer,
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
