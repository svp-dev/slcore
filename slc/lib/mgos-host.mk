libmgos_a_CFLAGS = -b $(COMMON_TAG)
libmgos_a_SOURCES = \
	../mgos/gfx/gfx.c \
	../mgos/init/mtinit.h \
	../mgos/init/mtinit.c \
	../mgos/init/initmain.c \
	../mgos/init/initplaces.c \
	../mgos/sep/mtsep.c 

AM_CPPFLAGS += \
	-I$(srcdir)/../mgos
