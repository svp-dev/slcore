SUBDIRS = slsys 

ACLOCAL_AMFLAGS = -I ../build-aux

include libc-headers.mk
dist_noinst_DATA = $(LIBCHEADERS:%=libc/%)

#### slc target trees

SUBDIRS += host-host-seqc 

if ENABLE_SLC_MTALPHA
SUBDIRS += mtalpha-sim
endif
if ENABLE_SLC_MIPSEL
SUBDIRS += mipsel-sim
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




