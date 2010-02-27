#include "mtconf.h"
#include <svp/sep.h>
#include <svp/testoutput.h>

extern int verbose_boot;

typedef void (*parserfunc)(confword_t* data);

static
void parse_arch_v1(confword_t* data)
{
    if (!verbose_boot)
        return;

    static const char * const unknown = "UNKNOWN";
    const char *archtype = unknown;
    const char *isatype = unknown;
    const char *memtype = unknown;
    switch(data[0])
    {
    case 1: archtype = "simulated microgrid"; break;
    }
    switch(data[1])
    {
    case 1: isatype = "DEC Alpha with MT extensions"; break;
    case 2: isatype = "SPARCV8 with MT extensions"; break;
    }
    switch(data[3])
    {
    case 1: memtype = "serial"; break;
    case 2: memtype = "parallel"; break;
    case 3: memtype = "banked"; break;
    case 4: memtype = "random banked"; break;
    case 5: memtype = "COMA network, ZL protocol"; break;
    case 6: memtype = "COMA network, ML protocol"; break;
    }
        
    output_string("architecture settings\n"
                  "   type: ", 2);
    output_string(archtype, 2);
    output_string("\n"
                  "   core ISA: ", 2);
    output_string(isatype, 2);
    output_string("\n"
                  "   memory: ", 2);
    output_string(memtype, 2);
    output_string("\n"
                  "   FPUs: ", 2);
    output_uint(data[2], 2);
    output_char('\n', 2);
}

static const confword_t one_ghz = 1000;
const confword_t * mgconf_core_freq = &one_ghz;

static
void parse_timings_v1(confword_t* data)
{
    mgconf_core_freq = &data[0];

    if (!verbose_boot)
        return;

    output_string("timings\n"
                  "   core frequency: ", 2);
    output_uint(data[0], 2);
    output_string("MHz\n"
                  "   external memory bandwidth: ", 2);
    output_uint(data[1], 2);
    output_string(" x 1E6 bytes/s\n", 2);
}

static
void parse_cache_v1(confword_t* data)
{
    if (!verbose_boot)
        return;

    output_string("cache settings\n"
                  "   cache line size: ", 2);
    output_uint(data[0], 2);
    output_string(" bytes\n"
                  "   L1 I-cache size: ", 2);
    output_uint(data[1], 2);
    output_string(" bytes\n"
                  "   L1 D-cache size: ", 2);
    output_uint(data[2], 2);
    output_string(" bytes\n"
                  "   L2 cache size: ", 2);
    output_uint(data[4], 2);
    output_string(" bytes\n"
                  "   number of L2 caches: ", 2);
    output_uint(data[3], 2);
    output_char('\n', 2);
}

static const confword_t def_ftes_ttes[2] = { 32, 256 };
const confword_t * mgconf_ftes_per_core = &def_ftes_ttes[0];
const confword_t * mgconf_ttes_per_core = &def_ftes_ttes[1];

static
void parse_conc_v1(confword_t* data)
{
    mgconf_ftes_per_core = &data[0];
    mgconf_ttes_per_core = &data[1];

    if (!verbose_boot)
        return;

    output_string("concurrency resources\n"
                  "   family entries per core: ", 2);
    output_uint(data[0], 2);
    output_string("\n"
                  "   thread entries per core: ", 2);
    output_uint(data[1], 2);
    output_string("\n"
                  "   int reg file size: ", 2);
    output_uint(data[2], 2);
    output_string("\n"
                  "   float reg file size: ", 2);
    output_uint(data[3], 2);
    output_char('\n', 2);
}

void * mgconf_layout_data;

static
void parse_layout_v1(confword_t* data)
{
    mgconf_layout_data = &data[0];
    if (!verbose_boot)
        return;

    output_string("place layout\n", 2);
}

static parserfunc parsers[] = 
{
    0,               // tag = 0 -> no configuration
    &parse_arch_v1, 
    &parse_timings_v1,
    &parse_cache_v1,
    &parse_conc_v1,
    &parse_layout_v1
};


void sys_conf_init(void *init_parameters)
{
    confword_t * words = (confword_t*) init_parameters;

    if (verbose_boot) {
        output_string("* reading configuration from 0x", 2);
        output_hex(words, 2);
        output_char(',', 2);
    }

    if (*words < 0x10000) 
    {
        if (verbose_boot) output_string(" old style: only place info found.\n", 2);
        mgconf_layout_data = init_parameters;
    }
    else 
    {
        if (verbose_boot) output_string(" new style:\n", 2);

        // new style microgrid, configuration blocks
        // are available, see MGSystem.cpp for values
        while (*words != 0)
        {
            unsigned tag = *words >> 16;
            unsigned size = *words & 0xffff;
            confword_t *data = words + 1;
            
            if (verbose_boot) {
                output_string("  at 0x", 2);
                output_hex(words, 2);
                output_string(", tag ", 2); output_uint(tag, 2);
                output_string(", ", 2); output_int(size, 2);
                output_string(" words: ", 2);
            }
            if (tag > (sizeof(parsers) / sizeof(parsers[0])) || !parsers[tag])
            {
                if (verbose_boot) output_string("unknown tag\n", 2);
            }
            else 
                parsers[tag](data);
          
            words += size + 1;
        }
    }
}
