## configure.ac: this file is part of the SL buildroot.
## 
## Copyright (C) 2009 The SL project
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## The complete GNU General Public Licence Notice can be found as the
## `COPYING' file in the root directory.
##

AC_PREREQ([2.61])
AC_INIT([sl], [2.1.0a], [sl-users@nic.surfnet.nl])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 tar-ustar no-define foreign dist-bzip2])

SLC_PATH_LOCAL([slc])
AC_WITH_SLC

AC_CONFIG_FILES([Makefile deploy/Makefile])
AC_CONFIG_SUBDIRS([slc])
AC_CONFIG_SUBDIRS([tests])
AC_CONFIG_SUBDIRS([programs])
AC_CONFIG_SUBDIRS([deploy/installer])

AC_OUTPUT

