#include <svp/testoutput.h>
#include <svp/delegate.h>
#include <svp/sep.h>
#include <stdlib.h>
#include "mtinit.h"
#include "mgsim/mtconf.h"

extern sl_place_t __main_place_id;

static noinline
void sys_set_main_pid(unsigned req_ncores, unsigned req_placesize)
{
    if (verbose_boot) output_string(" -> request to SEP...", 2);

    sl_place_t p;

    int r = sep_alloc(root_sep, &p, SAL_EXACT, req_placesize);
    if (r == -1) 
    {
        if (verbose_boot)
            output_string(" failed", 2);
        abort();
    }

    p = p & (p - 1); // drop the existing size bit
    p |= req_ncores; // add the new size bit
    
    __main_place_id = p;

    if (verbose_boot) 
    {
        output_string(" success, t_main goes to pid=0x", 2);
        output_hex(p, 2);
        output_ts(2);
        output_char('\n', 2);
    }
}

void sys_check_ncores(void)
{
    char *v = getenv("SLR_NCORES");
    char *v2 = getenv("SLR_PLACESIZE");
    if (v && v[0]) {
        if (verbose_boot) {
            output_string("* -n ", 2);
            output_string(v, 2);
        }
        unsigned req_ncores = strtoul(v, 0, 10);
        if (req_ncores == 0)
        {
            if (verbose_boot)
            {
                output_string(" -> t_main goes to pid=0x", 2);
                output_hex(__main_place_id = get_current_place(), 2);
                output_ts(2);
                output_char('\n', 2);
            }
        }
        else if ((req_ncores & (req_ncores - 1)) == 0)
        {
            unsigned req_placesize = 0;
            if (v2 && v2[0])
                req_placesize = strtoul(v2, 0, 10);
            req_placesize = req_placesize ? req_placesize : req_ncores;
            if (req_ncores > req_placesize)
                req_ncores = req_placesize;

            sys_set_main_pid(req_ncores, req_placesize);
        }
        else
        {
            if (verbose_boot) 
                output_string(" is invalid\n", 2);
            abort();
        }
    }
}
