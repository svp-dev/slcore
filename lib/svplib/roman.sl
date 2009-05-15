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

static struct roman_table_t {
  long base;
  const char* repr;
} roman_table[] = {
  {   50000, "(L)" },
  {   10000, "(X)" },
  {    5000, "(V)" },
  {    1000,   "M" },
  {     500,   "D" },
  {     100,   "C" },
  {      50,   "L" },
  {      10,   "X" },
  {       5,   "V" },
  {       1,   "I" },
  {       0,     0 }
};


sl_def(roman, void, sl_glparm(short, x))
{
  long num = sl_getp(x);
  if (num < 0) {
    __write1('-');
    num = -num;
  }

  struct roman_table_t *p = roman_table;
  const char *s;

  for (p = roman_table; p->base; ++p)
    while(num >= p->base) {
       for (s = p->repr; *s; ++s) __write1(*s);
       num = num - p->base;
    };
}
sl_enddef

