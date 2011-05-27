//
// gfx.h: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_SVP_GFX_H
# define SLC_SVP_GFX_H

#include <stddef.h>
#include <stdint.h>
#include <svp/compiler.h>

extern uint32_t * __restrict__ __gfx_framebuffer;
extern size_t __gfx_w;
extern size_t __gfx_h;

void gfx_resize(size_t, size_t);
void gfx_init(void);
void gfx_close(void);
void gfx_dump(unsigned key, int stream, int embed_ts, int embed_tinfo);

#define gfx_putpixel(X, Y, Data) do {					\
    size_t __x = (X), __y = (Y);					\
    size_t __w = __gfx_w;						\
    if (likely(likely((size_t)__x < __gfx_w) && likely((size_t)__y < __gfx_h))) \
      __gfx_framebuffer[__y * __w + __x] = (Data);			\
  } while(0)

#define gfx_fb_set(Offset, Data) do {					\
    size_t __o = (Offset);						\
    if (likely(__o < (__gfx_w*__gfx_h)))				\
      __gfx_framebuffer[__o] = (Data);					\
  } while(0)


#endif // ! SLC_SVP_GFX_H
