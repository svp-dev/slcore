#include "ca2d.h"
#include <svp/testoutput.h>

static
bool testrule(
    const size_t *birthcounts, size_t bc_l,
    const size_t *survivecounts, size_t sc_l,
    bool nw, bool nn, bool ne,
    bool ww, bool x, bool ee,
    bool sw, bool ss, bool se)
{
    int active = 
        (int)nw + (int)nn + 
        (int)ne + (int)ww + 
        (int)ee + (int)sw + 
        (int)ss + (int)se;
    if (x)
    {
        for (size_t i = 0; i < sc_l; ++i)
            if (active == survivecounts[i])
                return true;
        return false;
    }
    else
    {
        for (size_t i = 0; i < bc_l; ++i)
            if (active == birthcounts[i])
                return true;
        return false;
    }
}

void makerule(rule2D *rule, 
              const size_t *birthcounts, size_t bc_s,
              const size_t *survivecounts, size_t sc_s)
{
    int se, ss, sw, ee, x, ww, ne, nn, nw;
    for (size_t i = 0; i < 64; ++i)
        (*rule)[i] = 0;

    for (se = 0; se <= 1; ++se)
        for (ss = 0; ss <= 1; ++ss)
            for (sw = 0; sw <= 1; ++sw)
                for (ee = 0; ee <= 1; ++ee)
                    for (x = 0; x <= 1; ++x)
                        for (ww = 0; ww <= 1; ++ww)
                            for (ne = 0; ne <= 1; ++ne)
                                for (nn = 0; nn <= 1; ++nn)
                                    for (nw = 0; nw <= 1; ++nw)
                                    {
                                        int v = testrule(birthcounts, bc_s,
                                                         survivecounts, sc_s,
                                                         nw, nn, ne,
                                                         ww, x, ee,
                                                         sw, ss, se);
                                        size_t o1 =
                                            nw << 0 | nn << 1 | ne << 2;
                                        size_t o2 = 
                                            ww << 0 | x << 1 | ee << 2 |
                                            sw << 3 | ss << 4 | se << 5;
                                        (*rule)[o2] |= v << o1;
                                        // printf("Rule %zu %zu = %d\n", o2, o1, v);
                                    }
}
