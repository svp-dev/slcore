//
// sp1.c: this file is part of the SL toolchain.
//
// Copyright (C) 2011 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

sl_def(foo, void) {} sl_enddef

sl_def(t_main, void)
{
    sl_spawndecl(f1);
    sl_spawndecl(f2);
    
    sl_spawn(f1,,1,10,2,3,, foo);

    sl_spawn(f2,,23,10,-3,3,, foo);

    sl_spawnsync(f1);
    sl_spawnsync(f2);
}
sl_enddef
