########## muTC-PTL implementation ##########

nobase_dist_pkgdata_DATA += \
	host-host-ptl/include/svp_os.h

if ENABLE_SLC_PTL

nobase_pkglib_LIBRARIES += host-host-ptl/libsl.a 

host_host_ptl_libsl_a_SOURCES =
host_host_ptl_libsl_a_LIBADD = \
	host-host-ptl/io.o \
	host-host-ptl/roman.o \
	host-host-ptl/perf.o \
	host-host-ptl/gfx.o \
	host-host-ptl/div.o

SLC_PTL = $(SLC_RUN) -b ptl -nostdlib $(CXXFLAGS)

host-host-ptl/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) host-host-ptl
	$(slc_verbose)$(SLC_PTL) -c -o $@ $<

endif

CLEANFILES += \
	host-host-ptl/io.o \
	host-host-ptl/gfx.o \
	host-host-ptl/perf.o \
	host-host-ptl/div.o \
	host-host-ptl/roman.o


