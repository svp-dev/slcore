//
// args.c: this file is part of the SL toolchain.
//
// Copyright (C) 2009 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <unistd.h>
#include <stdlib.h>
#include <svp/testoutput.h>

/*
 * SLT_RUN: -- -a 10  -b -10
 */

int main(int argc, char **argv)
{
    int a = 0, b = 0;
    char ch;
    while ((ch = getopt(argc, argv, "a:b:")) != -1) 
        switch (ch) {
        case 'a':
            a = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        }

  int c = a + b;
  output_int(c, 1);
  output_char('\n', 1);
  return 0;
}

