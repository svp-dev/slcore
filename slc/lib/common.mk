RANLIB = $(SLRANLIB) $(COMMON_TAG)
AR     = $(SLAR) $(COMMON_TAG)
CC     = $(SLC) -nostdlib
CCAS   = $(SLC) -nostdlib
CCASFLAGS = -b $(COMMON_TAG)

pkglibdir = $(libdir)/sl-core/$(subdir)
sllibdir = $(pkglibdir)

sllib_DATA =
nobase_pkglib_LIBRARIES =
nobase_sllib_DATA =
nobase_dist_sllib_DATA =
nobase_nodist_pkgdata_DATA =
BUILT_SOURCES =
EXTRA_DIST =

AM_CPPFLAGS = \
	-Iinclude \
	-I$(srcdir)/include \
	-I$(srcdir)/../../tools/include \
	-I$(srcdir)/../slsys/include

LIBSL_SRC = \
	../slsys/deprecated/roman.c \
	../slsys/deprecated/div.c \
	../slsys/deprecated/io.c

noinst_LIBRARIES = libslcommon.a 

libslcommon_a_CFLAGS = -b $(COMMON_TAG)
libslcommon_a_SOURCES = \
	../slsys/perf/perf.c \
	../slsys/perf/perf_wrappers.c 
