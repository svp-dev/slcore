#include <svp/gfx.h>
#include <svp/testoutput.h>
#include <svp/compiler.h>
#include "mtconf.h"

uint32_t * __restrict__ __gfx_framebuffer = 0;
size_t __gfx_w = 0;
size_t __gfx_h = 0;

sl_def(do_gfx_init, sl__static)
{
    if (mg_gfx_ctl == 0)
    {
        output_string("gfx: no device available\n", 2);
        return;
    }

    /* set size 100x100 with 32bpp */
    mg_gfx_ctl[1] = 100;
    mg_gfx_ctl[2] = 100;
    mg_gfx_ctl[3] = 32;
    mg_gfx_ctl[0] = 1; // commit

    if (mg_gfx_ctl[1] != 100 ||
        mg_gfx_ctl[2] != 100 ||
        mg_gfx_ctl[3] != 32)
    {
        output_string("gfx: unable to set mode\n", 2);
        return;
    }
        
    __gfx_framebuffer = mg_gfx_fb;
    __gfx_w = __gfx_h = 100;
}
sl_enddef

void gfx_init(void)
{
    sl_create(,mg_io_place_id,,,,, sl__forcewait, do_gfx_init); sl_sync();
}

sl_def(do_gfx_resize, sl__static, sl_glparm(size_t, nw), sl_glparm(size_t, nh))
{
    if (mg_gfx_ctl == 0)
        return;

    mg_gfx_ctl[1] = sl_getp(nw);
    mg_gfx_ctl[2] = sl_getp(nh);
    mg_gfx_ctl[3] = 32;
    mg_gfx_ctl[0] = 1; // commit;

    __gfx_w = mg_gfx_ctl[1];
    __gfx_h = mg_gfx_ctl[2];

    if (__gfx_w != sl_getp(nw) || __gfx_h != sl_getp(nh))
    {
        output_string("gfx: warning: resolution changed to ", 2);
        output_uint(__gfx_w, 2);
        output_char('x', 2);
        output_uint(__gfx_h, 2);
        output_char('\n', 2);
    }
}
sl_enddef

void gfx_resize(size_t nw, size_t nh)
{
    sl_create(,mg_io_place_id,,,,,sl__forcewait, do_gfx_resize, sl_glarg(size_t,,nw), sl_glarg(size_t,,nh));
    sl_sync();
}

void gfx_close(void)
{ 
    __gfx_framebuffer = 0;
    __gfx_w = __gfx_h = 0;
}

sl_def(do_gfx_dump, sl__static, sl_glparm(unsigned, key), sl_glparm(int, stream), sl_glparm(int, embed_ts), sl_glparm(int, embed_tinfo))
{
    if (mg_gfx_ctl == 0)
        return;

    mg_gfx_ctl[5] = sl_getp(key);
    mg_gfx_ctl[4] = (sl_getp(stream) & 0xff) | (!!sl_getp(embed_ts) << 8);
}
sl_enddef

void gfx_dump(unsigned key, int stream, int embed_ts, int embed_tinfo)
{
    sl_create(, mg_io_place_id,,,,,, do_gfx_dump, 
              sl_glarg(unsigned,, key), 
              sl_glarg(int,, stream), 
              sl_glarg(int,, embed_ts), 
              sl_glarg(int,, embed_tinfo));
    sl_sync();
}

sl_def(do_gfx_putpixel, sl__static, sl_glparm(size_t, x), sl_glparm(size_t, y), sl_glparm(uint32_t, data))
{
    size_t w = __gfx_w;
    if (likely(likely(sl_getp(x) < w) && likely(sl_getp(y) < __gfx_h)))
        __gfx_framebuffer[sl_getp(y) * w + sl_getp(x)] = sl_getp(data);
}
sl_enddef

void gfx_putpixel(size_t x, size_t y, uint32_t data)
{
    sl_create(, mg_io_place_id,,,,,sl__forcewait, do_gfx_putpixel, sl_glarg(size_t,,x), sl_glarg(size_t,,y), sl_glarg(uint32_t,,data));
    sl_detach();
}

sl_def(do_gfx_fb_set, sl__static, sl_glparm(size_t, offset), sl_glparm(uint32_t, data))
{
    if (likely(sl_getp(offset) < (__gfx_w * __gfx_h)))
        __gfx_framebuffer[sl_getp(offset)] = sl_getp(data);
}
sl_enddef

void gfx_fb_set(size_t offset, uint32_t data)
{
    sl_create(, mg_io_place_id,,,,,sl__forcewait, do_gfx_fb_set, sl_glarg(size_t,,offset), sl_glarg(uint32_t,,data));
    sl_detach();
}


