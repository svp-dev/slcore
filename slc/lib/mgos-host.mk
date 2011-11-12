libmgos_a_CFLAGS = -b $(COMMON_TAG)
libmgos_a_SOURCES = \
	../mgos/gfx/gfx.c \
	../mgos/sep/dummysep.c

AM_CPPFLAGS += \
	-I$(srcdir)/../mgos
