libc_a_SOURCES += \
	../mgos/posix-compat/mtclock.c

libm_a_CFLAGS  = -b $(COMMON_TAG)
libm_a_SOURCES = \
	../libc/math/e_sqrtf.c \
	../libc/math/s_roundf.c \
	../libc/math/math_private.h

include ../smallmalloc.mk

libc_a_LIBADD += \
	$(libsmallmalloc_a_OBJECTS)
libc_a_DEPENDENCIES = $(libc_a_LIBADD)
