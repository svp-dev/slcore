#include <svp/gfx.h>
#include <svp/testoutput.h>
#include "mtconf.h"

uint32_t * __restrict__ __gfx_framebuffer = 0;
size_t __gfx_w = 0;
size_t __gfx_h = 0;

void gfx_init(void)
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

void gfx_resize(size_t nw, size_t nh)
{
    if (mg_gfx_ctl == 0)
        return;

    mg_gfx_ctl[1] = nw;
    mg_gfx_ctl[2] = nh;
    mg_gfx_ctl[3] = 32;
    mg_gfx_ctl[0] = 1; // commit;

    __gfx_w = mg_gfx_ctl[1];
    __gfx_h = mg_gfx_ctl[2];

    if (__gfx_w != nw || __gfx_h != nh)
    {
        output_string("gfx: warning: resolution changed to", 2);
        output_uint(__gfx_w, 2);
        output_char('x', 2);
        output_uint(__gfx_h, 2);
        output_char('\n', 2);
    }
}

void gfx_close(void)
{ 
    __gfx_framebuffer = 0;
    __gfx_w = __gfx_h = 0;
}

void gfx_dump(unsigned key, int stream, int embed_ts, int embed_tinfo /* unused */)
{
    if (mg_gfx_ctl == 0)
        return;

    mg_gfx_ctl[5] = key;
    mg_gfx_ctl[4] = (stream & 0xff) | (!!embed_ts << 8);
}
