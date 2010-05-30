########## Sequential-C implementation ##########

nobase_dist_pkgdata_DATA += \
	host-host-seqc/include/svp_os.h 


### Common rules ###

# define SEQC_TEMPLATE

# nobase_pkglib_LIBRARIES += \
# 	$(2)/libsl.a

# $(3)_libsl_a_BUILTOBJS = \
# 	$(2)/div.o \
# 	$(2)/roman.o \
# 	$(2)/perf.o \
# 	$(2)/gfx.o \
# 	$(2)/io.o

# $(3)_libsl_a_SOURCES = 
# $(3)_libsl_a_LIBADD = $($(3)_libsl_a_BUILTOBJS)

# SLC_CMD := $(SLC_RUN) -b $(1) $(AM_CFLAGS) $(CFLAGS)

# $(2)/%.o: $(srcdir)/src/%.c
# 	$(AM_V_at)$(MKDIR_P) $(2)
# 	$(slc_verbose)$(SLC_CMD) -c -o $@ $< 

# CLEANFILES += $($(3)_libsl_a_BUILTOBJS)

# endef

### "old" t-seqc ###

#$(eval $(call SEQC_TEMPLATE,seqc_o,seqc_o-host-host-seqc,seqc_o_host_host_seqc))

nobase_pkglib_LIBRARIES += \
	seqc_o-host-host-seqc/libsl.a

seqc_o_host_host_seqc_libsl_a_BUILTOBJS = \
	seqc_o-host-host-seqc/div.o \
	seqc_o-host-host-seqc/roman.o \
	seqc_o-host-host-seqc/perf.o \
	seqc_o-host-host-seqc/gfx.o \
	seqc_o-host-host-seqc/io.o

seqc_o_host_host_seqc_libsl_a_SOURCES = 
seqc_o_host_host_seqc_libsl_a_LIBADD = $(seqc_o_host_host_seqc_libsl_a_BUILTOBJS)

SLC_SEQC_O = $(SLC_RUN) -b seqc_o $(AM_CFLAGS) $(CFLAGS)

seqc_o-host-host-seqc/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) seqc_o-host-host-seqc
	$(slc_verbose)$(SLC_SEQC_O) -c -o $@ $< 

CLEANFILES += $(seqc_o_host_host_seqc_libsl_a_BUILTOBJS)

### "new" spr-seq ###

#$(eval $(call SEQC_TEMPLATE,seqc,seq_naked-host-host-seqc,seq_naked_host_host_seqc))


nobase_pkglib_LIBRARIES += \
	seq_naked-host-host-seqc/libsl.a

seq_naked_host_host_seqc_libsl_a_BUILTOBJS = \
	seq_naked-host-host-seqc/div.o \
	seq_naked-host-host-seqc/roman.o \
	seq_naked-host-host-seqc/perf.o \
	seq_naked-host-host-seqc/gfx.o \
	seq_naked-host-host-seqc/io.o

seq_naked_host_host_seqc_libsl_a_SOURCES = 
seq_naked_host_host_seqc_libsl_a_LIBADD = $(seq_naked_host_host_seqc_libsl_a_BUILTOBJS)

SLC_SEQC = $(SLC_RUN) -b seqc $(AM_CFLAGS) $(CFLAGS)

seq_naked-host-host-seqc/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) seq_naked-host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -c -o $@ $< 

CLEANFILES += $(seq_naked_host_host_seqc_libsl_a_BUILTOBJS)

