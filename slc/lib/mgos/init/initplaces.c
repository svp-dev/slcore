#include <svp/testoutput.h>
#include <svp/delegate.h>
#include <svp/sep.h>
#include <stdlib.h>
#include "mtinit.h"
#include "mgsim/mtconf.h"

sl_place_t __stdio_place_id;
sl_place_t __malloc_place_id;
sl_place_t __dtoa_place_id;

void sys_places_init(void)
{
    if (verbose_boot) output_string("* allocating places for the C stdlib...", 2);

    int r1 = sep_alloc(root_sep, &__stdio_place_id, SAL_MIN, 1);
    int r2 = sep_alloc(root_sep, &__malloc_place_id, SAL_MIN, 1);
    int r3 = sep_alloc(root_sep, &__dtoa_place_id, SAL_MIN, 1);
    if (r1 == -1 || r2 == -1 || r3 == -1)
    {
        if (verbose_boot) output_string(" failed\n", 2);
        abort();
    }

    if (verbose_boot)
    {
        output_string(" success: stdio at 0x", 2);
        output_hex(__stdio_place_id, 2);
        output_string(", malloc at 0x", 2);
        output_hex(__malloc_place_id, 2);
        output_string(", dtoa at 0x", 2);
        output_hex(__dtoa_place_id, 2);
        output_ts(2);
        output_char('\n', 2);        
    }
}
