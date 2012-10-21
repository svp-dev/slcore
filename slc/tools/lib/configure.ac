## configure.ac: this file is part of the SL toolchain.
## 
## Copyright (C) 2009,2011 The SL project
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## The complete GNU General Public Licence Notice can be found as the
## `COPYING' file in the root directory.
##

AC_PREREQ([2.65])

AC_INIT([sl-core], [lib], [sl-users@svp-home.org])

AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 tar-ustar no-define foreign dist-bzip2
                  silent-rules])

AM_SILENT_RULES([yes])

AC_PROG_SED
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_LN_S
AM_PROG_AS
SLC_TARGET_FLAGS

AC_CHECK_PROG([SLC], [set-by-parent-makefile])
AC_CHECK_PROG([SLAR], [set-by-parent-makefile])
AC_CHECK_PROG([SLRANLIB], [set-by-parent-makefile])

AC_CONFIG_FILES([Makefile
                 host-host-seqc/Makefile
                 host-host-ptl/Makefile
                 host-host-ptld/Makefile
                 host-host-hlsim/Makefile
                 host-host-hlsimd/Makefile
                 mtalpha-sim/Makefile
                 mipsel-sim/Makefile
                 mtsparc-sim/Makefile
                 mtsparc-fpga/Makefile
                 ])

AC_OUTPUT

