RANLIB = $(SLRANLIB) $(COMMON_TAG)
AR     = $(SLAR) $(COMMON_TAG)
CC     = $(SLC) -nostdlib
CCAS   = $(SLC) -nostdlib
CCASFLAGS = -b $(COMMON_TAG)
AM_CPPFLAGS =

pkglibdir = $(libdir)/sl-core/$(subdir)
sllibdir = $(pkglibdir)

noinst_LIBRARIES        = libslrt.a
nobase_sllib_DATA       = slrt.o
nobase_pkglib_LIBRARIES = libslc.a

libslrt_a_CFLAGS = -b $(COMMON_TAG)
libslrt_a_CCASFLAGS = -b $(COMMON_TAG)
libslrt_a_SOURCES = $(SLRT_SRC)

libslc_a_CFLAGS = -b $(COMMON_TAG)
libslc_a_SOURCES = $(LIBSLC_SRC)


.SECONDARY: libslrt_a-slrt.o
slrt.o: libslrt_a-slrt.o
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)cp libslrt_a-slrt.o $@

CLEANFILES = slrt.o

MAIN_SRC = ../common/main.c
