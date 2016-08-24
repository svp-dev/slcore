libmgos_a_CFLAGS = -b $(COMMON_TAG)
libmgos_a_SOURCES = \
	../mgos/gfx/mtgfx.c \
	../mgos/init/mtinit.h \
	../mgos/init/mtinit.c \
	../mgos/init/initenv.c \
	../mgos/init/initmain.c \
	../mgos/init/initplaces.c \
	../mgos/input/mtargs.c \
	../mgos/input/mtinput.c \
	../mgos/mgsim/RPCServiceDatabase.h \
	../mgos/mgsim/mgsim.h \
	../mgos/mgsim/mtconf.c \
	../mgos/mgsim/mtconf.h \
	../mgos/posix-compat/exit.c \
	../mgos/posix-compat/gtod.c \
	../mgos/posix-compat/gtod_virtual.c \
	../mgos/posix-compat/heap.c \
	../mgos/posix-compat/heap.h \
	../mgos/posix-compat/mtunix.c \
	../mgos/posix-compat/time.c \
	../mgos/posix-compat/time_virtual.c \
	../mgos/sep/mtsep.c \
	../mgos/tlstack_malloc.c \
	../mgos/uart/mtuart.c

AM_CPPFLAGS += \
	-I$(srcdir)/../mgos
