//
// gfx.c: this file is part of the SL toolchain.
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

#include <svp/gfx.h>
#include <svp/compiler.h>
#include <sys/resource.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

uint32_t * __restrict__ __gfx_framebuffer = 0;
size_t __gfx_w = 0;
size_t __gfx_h = 0;

void gfx_resize(size_t nw, size_t nh)
{
  if (unlikely(unlikely(!nw) || unlikely(!nh))) return;

  if (likely(__gfx_framebuffer))
    free(__gfx_framebuffer);

  __gfx_framebuffer = (uint32_t*)calloc(nw * nh, sizeof(uint32_t));

  if (likely(__gfx_framebuffer != 0)) {
    __gfx_w = nw;
    __gfx_h = nh;
  } else
    __gfx_w = __gfx_h = 0;
}

void gfx_init(void) { gfx_resize(100, 100); }
void gfx_close(void)
{
  if (likely(__gfx_framebuffer != 0)) free(__gfx_framebuffer);
  __gfx_framebuffer = 0;
  __gfx_w = __gfx_h = 0;
}

void gfx_dump(unsigned key, int stream, int embed_ts, int embed_tinfo /* unused */)
{
  char buffer[128];
  FILE *f = 0;

  if (unlikely(__gfx_framebuffer == 0))
    return;

  if (stream == 0) {
    if (embed_ts) {
      struct rusage r;
      getrusage(RUSAGE_SELF, &r);
      snprintf(buffer, 128, "gfx.%u.%ull%06u.ppm", key, (unsigned long long)r.ru_utime.tv_sec, (unsigned)r.ru_utime.tv_usec);
    } else
      snprintf(buffer, 128, "gfx.%u.ppm", key);
    f = fopen(buffer, "w");
  }
  else if (stream == 2)
    f = stderr;
  else if (stream == 1)
    f = stdout;

  if (!f)
    return;

  size_t w = __gfx_w, h = __gfx_h;
  size_t x, y;
  uint32_t * __restrict__ pixels = __gfx_framebuffer;
  fprintf(f, "P3\n#key: %u\n#\n%u %u 255\n", key, w, h);
  for (y = 0; y < h; ++y) {
    for (x = 0; x < w; ++x) {
      uint32_t d = pixels[y * w + x];
      fprintf(f, "%u %u %u ", (int)((d >> 16) & 0xff), (int)((d >> 8) & 0xff), (int)(d & 0xff));
    }
    fputc('\n', f);
  }
  if (stream == 0)
    fclose(f);
}
