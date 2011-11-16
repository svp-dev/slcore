
#include <svp/sep.h>

static
int root_sep_ctl(struct SEP* sep, unsigned long request, void *a, void *b)
{
    return -1;
}

void sep_dump_info(struct SEP* notused)
{
    /* do nothing */
}

static struct SEP root_sep_data = { &root_sep_ctl };
struct SEP *root_sep = &root_sep_data;
