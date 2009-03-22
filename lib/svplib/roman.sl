//                                                             -*- m4 -*-
// roman.sl: this file is part of the slc project.
//
// Copyright (C) 2009 The SL project.
// All rights reserved.
//
// $Id: roman.sl 1782 2009-03-18 13:12:05Z kena $
//
m4_include(svp/lib.slh)
m4_include(svp/roman.slh)

sl_def(roman, void, sl_glparm(short, x))
{
  int num = sl_getp(x);
  if (num < 0) {
    __write1('-');
    num = -num;
  }
  while(num >= 1000) {
     __write1('M');
     num = num - 1000;
  }
  while(num >= 500) {
     __write1('D');
     num = num - 500;
  }
  while(num >= 100) {
     __write1('C');
     num = num - 100;
  }
  while(num >= 50) {
     __write1('L');
     num = num - 50;
  }
  while(num >= 10) {
     __write1('X');
     num = num - 10;
  }
  while(num >= 5) {
     __write1('V');
     num = num - 5;
  }
  while(num--)
     __write1('I');
}
sl_enddef

