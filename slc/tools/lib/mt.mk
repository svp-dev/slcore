LIBSLC_SRC = \
	../common/fibre.c \
	../common/places.c \
	../common/divide.c \
	../common/abort.c \
	../mt/mgsim.c

AM_CPPFLAGS += -I$(srcdir)/../../../lib/libc/include
