SUBDIRS = host-host-seqc 

if ENABLE_SLC_MTALPHA
SUBDIRS += mtalpha-sim
endif
if ENABLE_SLC_MTSPARC
SUBDIRS += mtsparc-sim mtsparc-fpga
endif

if ENABLE_SLC_PTL
SUBDIRS += \
	host-host-ptl \
	host-host-ptld
endif

if ENABLE_SLC_HLSIM
SUBDIRS += \
	host-host-hlsim \
	host-host-hlsimd
endif

ACLOCAL_AMFLAGS = -I ../../build-aux

RUNSCRIPTS = r-host.sh r-mtalpha-sim.sh r-mtsparc-sim.sh r-mtsparc-fpga.sh
EXTRA_DIST = r-host.sh.in r-mt-sim.sh.in r-mtsparc-fpga.sh.in

pkglib_SCRIPTS = $(RUNSCRIPTS)
CLEANFILES = $(RUNSCRIPTS)

r-host.sh: r-host.sh.in Makefile
	$(AM_V_at)rm -f $@ $@.out
	$(AM_V_GEN)$(SED) <$< >$@.out \
	   -e 's|@BASH[@]|$(BASH)|g'
	$(AM_V_at)chmod +x,a-w $@.out
	$(AM_V_at)mv -f $@.out $@

r-mtalpha-sim.sh: r-mt-sim.sh.in Makefile
	$(AM_V_at)rm -f $@ $@.out
	$(AM_V_GEN)$(SED) <$< >$@.out \
	 -e 's|@slc_datadir[@]|$(pkgdatadir)|g' \
	 -e 's|@MGSIM_ARCH[@]|$(MGSIM_ALPHA)|g' \
	 -e 's|@NM_ARCH[@]|$(NM_MTALPHA)|g' \
	 -e 's|@ARCH[@]|ALPHA|g' \
	 -e 's|@arch[@]|alpha|g' \
	 -e 's|@BASH[@]|$(BASH)|g' \
	 -e 's|@python[@]|$(PYTHON)|g'
	$(AM_V_at)chmod +x,a-w $@.out
	$(AM_V_at)mv -f $@.out $@

r-mtsparc-sim.sh: r-mt-sim.sh.in Makefile
	$(AM_V_at)rm -f $@ $@.out
	$(AM_V_GEN)$(SED) <$< >$@.out \
	 -e 's|@slc_datadir[@]|$(pkgdatadir)|g' \
	 -e 's|@MGSIM_ARCH[@]|$(MGSIM_SPARC)|g' \
	 -e 's|@NM_ARCH[@]|$(NM_MTSPARC)|g' \
	 -e 's|@ARCH[@]|SPARC|g' \
	 -e 's|@arch[@]|sparc|g' \
	 -e 's|@BASH[@]|$(BASH)|g' \
	 -e 's|@python[@]|$(PYTHON)|g'
	$(AM_V_at)chmod +x,a-w $@.out
	$(AM_V_at)mv -f $@.out $@

r-mtsparc-fpga.sh: r-mtsparc-fpga.sh.in Makefile
	$(AM_V_at)rm -f $@ $@.out
	$(AM_V_GEN)$(SED) <$< >$@.out \
	 -e 's|@slc_datadir[@]|$(pkgdatadir)|g' \
	 -e 's|@NM_ARCH[@]|$(NM_MTSPARC)|g' \
	 -e 's|@ARCH[@]|SPARC|g' \
	 -e 's|@arch[@]|sparc|g' \
	 -e 's|@BASH[@]|$(BASH)|g' \
	 -e 's|@python[@]|$(PYTHON)|g'
	$(AM_V_at)chmod +x,a-w $@.out
	$(AM_V_at)mv -f $@.out $@


