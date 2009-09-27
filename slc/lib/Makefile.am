## Makefile.am: this file is part of the SL toolchain.
## 
## Copyright (C) 2009 The SL project
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## The complete GNU General Public Licence Notice can be found as the
## `COPYING' file in the root directory.
##

# 
EXTRA_DIST = \
	src/io.c \
	src/gfx.c \
	src/roman.c \
	src/mtinit.c \
	src/mtsep.c \
	src/div.c

nobase_dist_pkgdata_DATA = \
	include/cstdint.h \
	include/cstddef.h \
	include/cstdbool.h \
	include/climits.h \
	include/cmalloc.h \
	include/calloca.h \
	include/cmath.h \
	include/cfloat.h \
	include/sac_helpers.h \
	include/svp/argslot.h \
	include/svp/assert.h \
	include/svp/callgate.h \
	include/svp/div.h \
	include/svp/io.h \
	include/svp/gfx.h \
	include/svp/sep.h \
	include/svp/roman.h \
	include/svp/iomacros.h \
	include/svp/perf.h

include $(top_srcdir)/build-aux/sl.mk
nobase_pkglib_DATA = 
nobase_pkglib_LIBRARIES = 
CLEANFILES =
MAINTAINERCLEANFILES = 
BUILT_SOURCES = 

include $(srcdir)/seqc.mk
include $(srcdir)/ptl.mk
include $(srcdir)/mtalpha.mk

