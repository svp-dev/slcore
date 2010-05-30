########## Sequential-C implementation ##########

EXTRA_DIST += \
	host-host-seqc/slrt.c 


### Common rules ###

# define SEQC_TEMPLATE

# # arg 1 = slc target
# # arg 2 = libdir
# # arg 3 = libdir_undescores

# nobase_pkglib_DATA += \
# 	$(2)/slrt.o

# nobase_pkglib_LIBRARIES += \
# 	$(2)/libslc.a \
# 	$(2)/libslmain.a

# $(3)_libslc_a_SOURCES = empty.c # empty for now
# $(3)_libslmain_a_SOURCES = # empty for now
# $(3)_libslmain_a_LIBADD = $(2)/main.o

# SLC_CMD := $(SLC_RUN) -b $(1) -nostdlib -nodefaultmain

# $(2)/%.o: $(srcdir)/host-host-seqc/%.c
# 	$(AM_V_at)$(MKDIR_P) seqc_o-host-host-seqc
# 	$(slc_verbose)$(SLC_CMD) -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

# $(2)/%.o: $(srcdir)/%.c
# 	$(AM_V_at)$(MKDIR_P) seqc_o-host-host-seqc
# 	$(slc_verbose)$(SLC_CMD) -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

# CLEANFILES += \
# 	$(2)/main.o \
# 	$(2)/slrt.o 

# endef

### "old" t-seqc ###

nobase_dist_pkgdata_DATA += \
	t-seqc/slimpl.m4 \
	t-seqc/include/sl_support.h

#$(eval $(call SEQC_TEMPLATE,seqc_o,seqc_o-host-host-seqc,seqc_o_host_host_seqc))

nobase_pkglib_DATA += \
	seqc_o-host-host-seqc/slrt.o

nobase_pkglib_LIBRARIES += \
	seqc_o-host-host-seqc/libslc.a \
	seqc_o-host-host-seqc/libslmain.a

seqc_o_host_host_seqc_libslc_a_SOURCES = empty.c # empty for now
seqc_o_host_host_seqc_libslmain_a_SOURCES = # empty for now
seqc_o_host_host_seqc_libslmain_a_LIBADD = seqc_o-host-host-seqc/main.o

SLC_SEQC_O = $(SLC_RUN) -b seqc_o 

seqc_o-host-host-seqc/%.o: $(srcdir)/host-host-seqc/%.c
	$(AM_V_at)$(MKDIR_P) seqc_o-host-host-seqc
	$(slc_verbose)$(SLC_SEQC_O) -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

seqc_o-host-host-seqc/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) seqc_o-host-host-seqc
	$(slc_verbose)$(SLC_SEQC_O) -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

CLEANFILES += \
	seqc_o-host-host-seqc/main.o \
	seqc_o-host-host-seqc/slrt.o 


### "new" spr-seq ###

#$(eval $(call SEQC_TEMPLATE,seqc,seq_naked-host-host-seqc,seq_naked_host_host_seqc))


nobase_pkglib_DATA += \
	seq_naked-host-host-seqc/slrt.o

nobase_pkglib_LIBRARIES += \
	seq_naked-host-host-seqc/libslc.a \
	seq_naked-host-host-seqc/libslmain.a

seq_naked_host_host_seqc_libslc_a_SOURCES = empty.c # empty for now
seq_naked_host_host_seqc_libslmain_a_SOURCES = # empty for now
seq_naked_host_host_seqc_libslmain_a_LIBADD = seq_naked-host-host-seqc/main.o

SLC_SEQC = $(SLC_RUN) -b seqc -nostdlib -nodefaultmain

seq_naked-host-host-seqc/%.o: $(srcdir)/host-host-seqc/%.c
	$(AM_V_at)$(MKDIR_P) seqc_o-host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

seq_naked-host-host-seqc/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) seqc_o-host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

CLEANFILES += \
	seq_naked-host-host-seqc/main.o \
	seq_naked-host-host-seqc/slrt.o 


