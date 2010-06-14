########## muTC-PTL implementation ##########

nobase_dist_pkgdata_DATA += \
	host-host-hrt/include/sl_hrt.h

EXTRA_DIST += \
	host-host-hrt/tc.c

if ENABLE_SLC_HRT

### naked target

nobase_pkglib_DATA += \
	hrt_naked-host-host-hrt/slrt.o

nobase_pkglib_LIBRARIES += \
	hrt_naked-host-host-hrt/libslc.a \
	hrt_naked-host-host-hrt/libslmain.a 

hrt_naked_host_host_hrt_libslc_a_SOURCES = host-host-hrt/tc.c
hrt_naked_host_host_hrt_libslmain_a_SOURCES = # empty for now
hrt_naked_host_host_hrt_libslmain_a_LIBADD = hrt_naked-host-host-hrt/main.o

hrt_naked_host_host_hrt_libslc_a_CPPFLAGS = \
	-I$(srcdir)/host-host-hrt/include \
	-I$(builddir)/host-host-hrt/include

SLC_HRT_N = $(SLC_RUN) -b hrt_n -nostdlib

hrt_naked-host-host-hrt/%.o: $(srcdir)/host-host-hrt/%.c
	$(AM_V_at)$(MKDIR_P) hrt_naked-host-host-hrt
	$(slc_verbose)$(SLC_HRT_N) -c -o $@ $< $(AM_CXXFLAGS) $(CXXFLAGS)

hrt_naked-host-host-hrt/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) hrt_naked-host-host-hrt
	$(slc_verbose)$(SLC_HRT_N) -c -o $@ $< $(AM_CXXFLAGS) $(CXXFLAGS)

CLEANFILES += \
	hrt_naked-host-host-hrt/main.o \
	hrt_naked-host-host-hrt/slrt.o

endif

