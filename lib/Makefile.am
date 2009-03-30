# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 
EXTRA_DIST = \
	svplib/io.sl \
	svplib/roman.sl \
	svplib/div.sl

nobase_dist_pkglib_SCRIPTS = \
	r-host/run.sh

nobase_pkglib_SCRIPTS = \
	r-mtalpha-sim/run.sh \
	r-mtsparc-sim/run.sh


nobase_dist_pkgdata_DATA = \
	t-utc/slimpl.m4 \
	t-utc0/slimpl.m4 \
	t-utc0/include/sl_support.h 

include $(top_srcdir)/tests.mk
nobase_pkglib_DATA = 
nobase_pkglib_LIBRARIES = 
CLEANFILES =
MAINTAINERCLEANFILES = 
BUILT_SOURCES = 

include $(srcdir)/seqc.mk
include $(srcdir)/ptl.mk
include $(srcdir)/mtalpha.mk


