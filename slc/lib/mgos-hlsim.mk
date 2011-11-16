libmgos_a_CFLAGS = -b $(COMMON_TAG)
libmgos_a_SOURCES = \
	../mgos/gfx/gfx.c \
	../mgos/sep/mtsep.c

AM_CPPFLAGS += \
	-I$(srcdir)/../mgos
