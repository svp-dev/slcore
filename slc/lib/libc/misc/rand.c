//
// rand.c: this file is part of the SL toolchain.
//
// Copyright (C) 2015 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <stdlib.h>

static int do_rand(unsigned long *ctx)
{
    /*
     * Compute x = (7^5 * x) mod (2^31 - 1)
     * without overflowing 31 bits:
     *      (2^31 - 1) = 127773 * (7^5) + 2836
     * From "Random number generators: good ones are hard to find",
     * Park and Miller, Communications of the ACM, vol. 31, no. 10,
     * October 1988, p. 1195.
     */
    long hi, lo, x;

    /* Must be in [1, 0x7ffffffe] range at this point. */
    hi = *ctx / 127773;
    lo = *ctx % 127773;
    x = 16807 * lo - 2836 * hi;
    if (x < 0)
        x += 0x7fffffff;
    *ctx = x;
    /* Transform to [0, 0x7ffffffd] range. */
    return (x - 1);
}

// FIXME: this should perhaps be protected against data races?

static unsigned long next = 2;
int rand(void)
{
    return do_rand(&next);
}

void srand(unsigned seed)
{
    next = seed;
    /* Transform to [1, 0x7ffffffe] range. */
    next = (next % 0x7ffffffe) + 1;
}
