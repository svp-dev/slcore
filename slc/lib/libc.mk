include ../libc-headers.mk
include ../gdtoa.mk

BUILT_SOURCES += include/assert.h

include/assert.h:
	$(LN_S) $(srcdir)/../libc/include include

headerdir = $(pkgdatadir)/$(subdir)
nobase_nodist_header_DATA = $(LIBCHEADERS)


libc_a_CFLAGS = -b $(COMMON_TAG)
libc_a_LIBADD = 
libc_a_SOURCES = \
	$(GDTOA_SRC) \
	../libc/bsd/libc/gdtoa/_hdtoa.c \
	../libc/bsd/libc/gen/errlst.c \
	../libc/bsd/libc/include/fpmath.h \
	../libc/bsd/libc/stdio/floatio.h \
	../libc/bsd/libc/stdio/fprintf.c \
	../libc/bsd/libc/stdio/printf-pos.c \
	../libc/bsd/libc/stdio/printf.c \
	../libc/bsd/libc/stdio/printfcommon.h \
	../libc/bsd/libc/stdio/printflocal.h \
	../libc/bsd/libc/stdio/sprintf.c \
	../libc/bsd/libc/stdio/vfprintf.c \
	../libc/bsd/libc/stdlib/atoll.c \
	../libc/bsd/libc/stdlib/atoi.c \
	../libc/bsd/libc/stdlib/atol.c \
	../libc/bsd/libc/stdlib/getenv.c \
	../libc/bsd/libc/stdlib/strtol.c \
	../libc/bsd/libc/stdlib/strtoll.c \
	../libc/bsd/libc/stdlib/strtoul.c \
	../libc/bsd/libc/stdlib/strtoull.c \
	../libc/bsd/libc/string/memchr.c \
	../libc/bsd/libc/string/memcmp.c \
	../libc/bsd/libc/string/memcpy.c \
	../libc/bsd/libc/string/memmove.c \
	../libc/bsd/libc/string/memset.c \
	../libc/bsd/libc/string/stpcpy.c \
	../libc/bsd/libc/string/stpncpy.c \
	../libc/bsd/libc/string/strcat.c \
	../libc/bsd/libc/string/strchr.c \
	../libc/bsd/libc/string/strcmp.c \
	../libc/bsd/libc/string/strcpy.c \
	../libc/bsd/libc/string/strdup.c \
	../libc/bsd/libc/string/strerror.c \
	../libc/bsd/libc/string/strlcat.c \
	../libc/bsd/libc/string/strlcpy.c \
	../libc/bsd/libc/string/strlen.c \
	../libc/bsd/libc/string/strncat.c \
	../libc/bsd/libc/string/strncmp.c \
	../libc/bsd/libc/string/strncpy.c \
	../libc/bsd/libc/string/strnlen.c \
	../libc/bsd/libc/string/bcopy.c \
	../libc/bsd/libc/string/bzero.c \
	../libc/malloc/mtmalloc.h \
	../libc/malloc/malloc_excl.c \
	../libc/misc/ctype.c \
	../libc/misc/dtoa_simple.c \
	../libc/misc/errno.c \
	../libc/stdio/fputc.c \
	../libc/stdio/fputs.c \
	../libc/stdio/fwrite.c \
	../libc/stdio/mtstdio.c \
	../libc/stdio/mtstdio.h \
	../libc/stdio/perror.c \
	../libc/stdio/putc.c \
	../libc/stdio/putchar.c \
	../libc/stdio/puts.c \
	../libc/stdio/snprintf.c \
	../libc/stdio/vprintf.c \
	../libc/stdio/vsnprintf.c \
	../libc/string/ffs.c

AM_CPPFLAGS += \
	-I$(srcdir)/../libc/bsd/libc/include \
	-I$(srcdir)/../libc/bsd/libc/stdio \
	-I$(srcdir)/../libc/bsd/contrib \
	-I$(srcdir)/../libc
