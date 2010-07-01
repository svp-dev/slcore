//
// mutable.c: this file is part of the SL toolchain.
//
// Copyright (C) 2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

sl_def(foo, void, sl_glparm_mutable(int, x), sl_glfparm_mutable(float, y))
{
    sl_getp(x) = 3;
    sl_getp(y) = 4.5;
}
sl_enddef

// XIGNORE: ptl*:*

sl_def(t_main, void)
{
    sl_proccall(foo, sl_glarg(int, , 3), sl_glfarg(float, , 4.5));
}
sl_enddef
