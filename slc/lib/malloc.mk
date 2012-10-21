noinst_LIBRARIES += \
	libmalloc.a \
	libmalloc_debug.a

sllib_DATA += \
	$(libmalloc_a_OBJECTS) \
	$(libmalloc_debug_a_OBJECTS)

MALLOC_DEFS = \
	-DUSE_DL_PREFIX \
	-DHAVE_MMAP=0 \
	-DMALLOC_FAILURE_ACTION="" \
	-DLACKS_UNISTD_H \
	-DUSE_BUILTIN_FFS=1 \
	-DLACKS_SYS_PARAM_H \
	-DLACKS_TIME

MALLOC_DEFS_DEBUG = \
	-DDEBUG=1 \
	-DABORT_ON_ASSERT_FAILURE=0 \
	-DFOOTERS=1

libmalloc_a_SOURCES = ../libc/malloc/malloc.c
libmalloc_a_CPPFLAGS = $(AM_CPPFLAGS) $(MALLOC_DEFS) $(MALLOC_DEFS_TARGET)
libmalloc_a_CFLAGS = -b $(COMMON_TAG)

libmalloc_debug_a_SOURCES = ../libc/malloc/malloc.c
libmalloc_debug_a_CPPFLAGS = $(AM_CPPFLAGS) $(MALLOC_DEFS) $(MALLOC_DEFS_TARGET) $(MALLOC_DEFS_DEBUG)
libmalloc_debug_a_CFLAGS = -b $(COMMON_TAG)
