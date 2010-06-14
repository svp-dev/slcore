## Makefile.am: this file is part of the SL toolchain.
## 
## Copyright (C) 2009,2010 The SL project
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
SUBDIRS = src/tlsmalloc
 
EXTRA_DIST = \
	src/io.c \
	src/gfx.c \
	src/perf.c \
	src/roman.c \
	src/mtinit.c \
	src/mtclock.c \
	src/mtsep.c \
	src/div.c

nobase_dist_pkgdata_DATA = \
	include/sys/ctypes.h \
	include/calloca.h \
	include/cassert.h \
	include/cctype.h \
	include/cerrno.h \
	include/cfloat.h \
	include/climits.h \
	include/cmalloc.h \
	include/cmath.h \
	include/cstdbool.h \
	include/cstdarg.h \
	include/cstddef.h \
	include/cstdint.h \
	include/cstdio.h \
	include/cstdlib.h \
	include/cstring.h \
	include/cstrings.h \
	include/ctime.h \
	include/sac_helpers.h \
	include/undocumented.h \
	include/svp/argslot.h \
	include/svp/assert.h \
	include/svp/callgate.h \
	include/svp/div.h \
	include/svp/fast_malloc.h \
	include/svp/gfx.h \
	include/svp/io.h \
	include/svp/iomacros.h \
	include/svp/perf.h \
	include/svp/roman.h \
	include/svp/sep.h

include $(top_srcdir)/build-aux/sl.mk
nobase_pkglib_DATA = 
nobase_pkglib_LIBRARIES = 
CLEANFILES =
MAINTAINERCLEANFILES = 
BUILT_SOURCES = 

include $(srcdir)/seqc.mk
include $(srcdir)/ptl.mk
include $(srcdir)/mtalpha.mk

