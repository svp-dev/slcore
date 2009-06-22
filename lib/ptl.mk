########## muTC-PTL implementation ##########

nobase_dist_pkgdata_DATA += \
	host-host-ptl/include/svp_os.h

nobase_pkglib_LIBRARIES += host-host-ptl/libsl.a 

host_host_ptl_libsl_a_SOURCES =
host_host_ptl_libsl_a_LIBADD = \
	host-host-ptl/io.o \
	host-host-ptl/roman.o \
	host-host-ptl/div.o

SLC_PTL = $(SLC) -b ptl -nostdlib $(CXXFLAGS)

host-host-ptl/io.o: $(srcdir)/src/io.sl
	$(AM_V_at)$(MKDIR_P) host-host-ptl
	$(slc_verbose)$(SLC_PTL) -c -o $@ $<

host-host-ptl/div.o: $(srcdir)/src/div.sl
	$(AM_V_at)$(MKDIR_P) host-host-ptl
	$(slc_verbose)$(SLC_PTL) -c -o $@ $<

host-host-ptl/roman.o: $(srcdir)/src/roman.sl
	$(AM_V_at)$(MKDIR_P) host-host-ptl
	$(slc_verbose)$(SLC_PTL) -c -o $@ $<

CLEANFILES += \
	host-host-ptl/io.o \
	host-host-ptl/div.o \
	host-host-ptl/roman.o


