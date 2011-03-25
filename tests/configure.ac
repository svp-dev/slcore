## configure.ac: this file is part of the SL test suite.
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

AC_PREREQ([2.61])
AC_INIT([sl-tests], 
        m4_esyscmd([build-aux/git-version-gen build-aux/tarball-version s/sl-tests-/v/ sl-tests]), 
        [sl-users@nic.surfnet.nl])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 tar-ustar no-define foreign dist-bzip2
                  color-tests silent-rules parallel-tests])

AM_SILENT_RULES([yes])

AC_WITH_SLC
AC_SLC_TESTS

AC_CONFIG_FILES([Makefile])

AC_OUTPUT

