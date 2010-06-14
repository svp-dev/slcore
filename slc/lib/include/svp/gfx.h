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

#ifdef __mt_freestanding__

#define gfx_init() ((void)0)
#define gfx_close() ((void)0)

#if defined(__alpha__) || defined(__mtalpha__)

#define gfx_resize(W, H) \
  __asm__ __volatile__("print %1, %0" : : "rI"((1<<5)|(1<<6)), "r"((((unsigned long)(W)) << 48) | ((((unsigned long)(H)) & 0xffff) << 32)))


#define gfx_putpixel(X, Y, Data)					\
  __asm__ __volatile__("print %1, %0" : : "rI"((1<<5)|(0<<6)),		\
		       "r"((((unsigned long)(X)) << 48) | ((((unsigned long)(Y)) & 0xffff) << 32) | ((Data) & 0xffffffff)))

#define gfx_fb_set(Offset, Data)					\
  __asm__ __volatile__("print %1, %0" : : "rI"((1<<5)|(0<<6)|(1<<4)),	\
		       "r"((((unsigned long)(Offset)) << 32) | ((Data) & 0xffffffff)))

#define gfx_dump(Key, Stream, EmbedTimeStamp, EmbedThreadInfo)		\
  __asm__ __volatile__("print %1, %0" : : "rI"((1<<5)|(2<<6)|((EmbedTimeStamp)<<4)|((EmbedThreadInfo)<<3)|(Stream)), "r"((long)(Key)))

#endif
#else

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

#endif


#endif // ! SLC_SVP_GFX_H
