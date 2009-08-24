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
dist_pkglib_SCRIPTS = \
	r-host.sh

pkglib_SCRIPTS = \
	r-mtalpha-sim.sh \
	r-mtsparc-sim.sh

nobase_dist_pkgdata_DATA = \
	arggen.c \
	t-utc/slimpl.m4 \
	t-utc0/slimpl.m4 \
	t-utc0/include/sl_support.h 

include $(top_srcdir)/sl.mk
nobase_pkglib_DATA =
nobase_pkglib_LIBRARIES = 
EXTRA_DIST =
CLEANFILES =
MAINTAINERCLEANFILES = 
BUILT_SOURCES = 

include $(srcdir)/seqc.mk
include $(srcdir)/ptl.mk
include $(srcdir)/mtalpha.mk
