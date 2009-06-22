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
	src/io.sl \
	src/roman.sl \
	src/div.sl

nobase_dist_pkgdata_DATA = \
	include/cstdint.slh \
	include/cstddef.slh \
	include/cstdbool.slh \
	include/climits.slh \
	include/cmalloc.slh \
	include/sac_helpers.slh \
	include/svp/argslot.slh \
	include/svp/assert.slh \
	include/svp/div.slh \
	include/svp/io.slh \
	include/svp/roman.slh \
	include/svp/iomacros.slh \
	include/svp/perf.slh

include $(top_srcdir)/sl.mk
nobase_pkglib_DATA = 
nobase_pkglib_LIBRARIES = 
CLEANFILES =
MAINTAINERCLEANFILES = 
BUILT_SOURCES = 

include $(srcdir)/seqc.mk
include $(srcdir)/ptl.mk
include $(srcdir)/mtalpha.mk

SUBDIRS = tests
