//
// ws3.c: this file is part of the SL toolchain.
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

sl_def(foo1, void,
       sl_shparm(
           int ,
           x
           )
    )
{
    sl_setp(x, sl_getp(x));
} 
sl_enddef 

sl_def(foo2, void, sl_shparm(int, x))
{
    int lx = sl_getp(x
        );
    sl_setp(x, 
lx
);
} 
sl_enddef 

sl_def(foo3, void, sl_shparm(int, x))
{
    int lx = sl_getp(  x);
    sl_setp(  x, lx);
} 
sl_enddef 

sl_def(foo4, void, sl_shparm(int, x))
{
    int lx = sl_getp(x  );
    sl_setp(x  , lx);
} 
sl_enddef 

sl_def(foo5, void, sl_shparm(int, x))
{
    int lx = sl_getp(
x
        );
    sl_setp(
x  
, lx
);
} 
sl_enddef 

sl_def(t_main, void)
{
    sl_create(,,,,,,, foo1, sl_sharg(int, , 0)); sl_sync();
    sl_create(,,,,,,, foo2, sl_sharg(int, , 0)); sl_sync();
    sl_create(,,,,,,, foo3, sl_sharg(int, , 0)); sl_sync();
    sl_create(,,,,,,, foo4, sl_sharg(int, , 0)); sl_sync();
    sl_create(,,,,,,, foo5, sl_sharg(int, , 0)); sl_sync();
}
sl_enddef
