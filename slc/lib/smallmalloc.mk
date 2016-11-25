SMALLMALLOC_DEFS = \
	-Dsmall_malloc=malloc \
	-Dsmall_free=free \
	-Dsmall_realloc=realloc \
	-Dsmall_calloc=calloc

noinst_LIBRARIES += \
	libsmallmalloc.a

sllib_DATA += \
	$(libsmallmalloc_a_OBJECTS)

libsmallmalloc_a_SOURCES = ../mgos/smallmalloc/smallmalloc.c
libsmallmalloc_a_CPPFLAGS = $(AM_CPPFLAGS) $(SMALLMALLOC_DEFS) 
libsmallmalloc_a_CFLAGS = -b $(COMMON_TAG)

