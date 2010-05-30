#! /usr/bin/env python
#
# spp: This file is part of the SL toolchain.
#
# Copyright (C) 2009,2010 The SL project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `COPYING' file in the root directory.
#

import pprint
import sys
import subprocess
import re
import os

from slc.msg import die, warn, log
from slc.mtalpha import regmagic
from slc.mtalpha import compile
from slc.mtalpha.asmproc import common, base, opt

if __name__ == "__main__":
   module = sys.argv[1]
   function = sys.argv[2]
   rest = sys.argv[3:]
   assert module in ['ppp2alpha', 'spr']
   assert function in ['compile','filter','filter2']
   if function == 'compile':
      compile.compile(*rest)
   elif function == 'filter':
      base.filter(*rest)
   elif function == 'filter2':
      opt.filter(*rest)

