TLSMALLOC_DEFS = \
	-Dtls_malloc=malloc \
	-Dtls_free=free \
	-Dtls_realloc=realloc \
	-Dtls_calloc=calloc

noinst_LIBRARIES += \
	libtlsmalloc_fast.a \
	libtlsmalloc.a \
	libtlsmalloc_debug.a \
	libtlsmalloc_mgdebug.a \
	libtlsmalloc_fast_nogc.a \
	libtlsmalloc_nogc.a \
	libtlsmalloc_nogc_debug.a \
	libtlsmalloc_nogc_mgdebug.a

sllib_DATA += \
	$(libtlsmalloc_fast_a_OBJECTS) \
	$(libtlsmalloc_a_OBJECTS) \
	$(libtlsmalloc_debug_a_OBJECTS) \
	$(libtlsmalloc_mgdebug_a_OBJECTS) \
	$(libtlsmalloc_fast_nogc_a_OBJECTS) \
	$(libtlsmalloc_nogc_a_OBJECTS) \
	$(libtlsmalloc_nogc_debug_a_OBJECTS) \
	$(libtlsmalloc_nogc_mgdebug_a_OBJECTS)

libtlsmalloc_fast_a_SOURCES = ../mgos/tlsmalloc/tlsmalloc.c
libtlsmalloc_fast_a_CPPFLAGS = $(AM_CPPFLAGS) $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_TARGET) -DNDEBUG
libtlsmalloc_fast_a_CFLAGS = -b $(COMMON_TAG)
libtlsmalloc_a_SOURCES = ../mgos/tlsmalloc/tlsmalloc.c
libtlsmalloc_a_CPPFLAGS = $(AM_CPPFLAGS) $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_TARGET) -DFOOTERS
libtlsmalloc_a_CFLAGS = -b $(COMMON_TAG)
libtlsmalloc_debug_a_SOURCES = ../mgos/tlsmalloc/tlsmalloc.c
libtlsmalloc_debug_a_CPPFLAGS = $(AM_CPPFLAGS) $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_TARGET) -DDEBUG_MALLOC -DFOOTERS
libtlsmalloc_debug_a_CFLAGS = -b $(COMMON_TAG)
libtlsmalloc_mgdebug_a_SOURCES = ../mgos/tlsmalloc/tlsmalloc.c
libtlsmalloc_mgdebug_a_CPPFLAGS = $(AM_CPPFLAGS) $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_TARGET) -DDEBUG_MGSIM -DFOOTERS
libtlsmalloc_mgdebug_a_CFLAGS = -b $(COMMON_TAG)

libtlsmalloc_fast_nogc_a_SOURCES = ../mgos/tlsmalloc/tlsmalloc.c
libtlsmalloc_fast_nogc_a_CPPFLAGS = $(AM_CPPFLAGS) $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_TARGET) -DNDEBUG -DDISABLE_GC
libtlsmalloc_fast_nogc_a_CFLAGS = -b $(COMMON_TAG)
libtlsmalloc_nogc_a_SOURCES = ../mgos/tlsmalloc/tlsmalloc.c
libtlsmalloc_nogc_a_CPPFLAGS = $(AM_CPPFLAGS) $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_TARGET) -DFOOTERS -DDISABLE_GC
libtlsmalloc_nogc_a_CFLAGS = -b $(COMMON_TAG)
libtlsmalloc_nogc_debug_a_SOURCES = ../mgos/tlsmalloc/tlsmalloc.c
libtlsmalloc_nogc_debug_a_CPPFLAGS = $(AM_CPPFLAGS) $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_TARGET) -DDEBUG_MALLOC -DFOOTERS -DDISABLE_GC
libtlsmalloc_nogc_debug_a_CFLAGS = -b $(COMMON_TAG)
libtlsmalloc_nogc_mgdebug_a_SOURCES = ../mgos/tlsmalloc/tlsmalloc.c
libtlsmalloc_nogc_mgdebug_a_CPPFLAGS = $(AM_CPPFLAGS) $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_TARGET) -DDEBUG_MGSIM -DFOOTERS -DDISABLE_GC
libtlsmalloc_nogc_mgdebug_a_CFLAGS = -b $(COMMON_TAG)

