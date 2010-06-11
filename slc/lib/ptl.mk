########## muTC-PTL implementation ##########

if ENABLE_SLC_PTL

nobase_pkglib_LIBRARIES += ptl-host-host-ptl/libsl.a 

ptl_host_host_ptl_libsl_a_BUILTOBJS = \
	ptl-host-host-ptl/io.o \
	ptl-host-host-ptl/roman.o \
	ptl-host-host-ptl/perf.o \
	ptl-host-host-ptl/gfx.o \
	ptl-host-host-ptl/div.o

ptl_host_host_ptl_libsl_a_SOURCES =
ptl_host_host_ptl_libsl_a_LIBADD = $(ptl_host_host_ptl_libsl_a_BUILTOBJS)

SLC_PTL = $(SLC_RUN) -b ptl -nostdlib $(CXXFLAGS)

ptl-host-host-ptl/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) ptl-host-host-ptl
	$(slc_verbose)$(SLC_PTL) -c -o $@ $<

CLEANFILES += $(ptl_host_host_ptl_libsl_a_BUILTOBJS)

endif



