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
SUBDIRS = mgos/tlsmalloc
 
EXTRA_DIST = \
	mgos/init/mtinit.c \
	mgos/input/mtinput.c \
	mgos/posix-compat/mtclock.c \
	mgos/sep/dummysep.c \
	mgos/sep/mtsep.c \
	mgos/uart/mtuart.c \
	slsys/deprecated/div.c \
	slsys/deprecated/io.c \
	slsys/deprecated/roman.c \
	slsys/gfx/gfx.c \
	slsys/perf/perf.c \
	slsys/perf/perf_wrappers.c

nobase_dist_pkgdata_DATA = \
	include/svp/deprecated/argslot.h \
	include/svp/deprecated/assert.h \
	include/svp/deprecated/callgate.h \
	include/svp/deprecated/div.h \
	include/svp/deprecated/fast_malloc.h \
	include/svp/deprecated/io.h \
	include/svp/deprecated/iomacros.h \
	include/svp/deprecated/roman.h \
	include/svp/gfx.h \
	include/svp/perf.h \
	include/svp/sep.h \
	include/sys/ctypes.h \
	include/sac_helpers.h \
	include/undocumented.h

include $(top_srcdir)/build-aux/sl.mk
pkglib_DATA = 
nobase_pkglib_DATA = 
nobase_pkglib_LIBRARIES = 
CLEANFILES =
MAINTAINERCLEANFILES = 
BUILT_SOURCES = 

include $(srcdir)/seqc.mk
include $(srcdir)/ptl.mk
include $(srcdir)/mtalpha.mk
include $(srcdir)/mtsparc.mk
include $(srcdir)/mtsparc2.mk

