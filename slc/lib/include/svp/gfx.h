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
void gfx_putpixel(size_t, size_t, uint32_t);
void gfx_fb_set(size_t, uint32_t);

#endif // ! SLC_SVP_GFX_H
