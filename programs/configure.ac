## configure.ac: this file is part of the SL demo suite.
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
AC_INIT([sl-programs], 
        m4_esyscmd([(pre=; if test -d ../.git -a ! -d .git; then pre=$PWD/; cd ..; fi; \
                    ${pre}build-aux/version-gen ${pre}build-aux/tarball-version ${pre}build-aux/package-version)]), 
        [sl-users@svp-home.org])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 tar-ustar no-define foreign dist-bzip2
                  color-tests silent-rules parallel-tests])

AM_SILENT_RULES([yes])

AC_PROG_EGREP
AC_PROG_SED
AC_PROG_CC
AC_PROG_RANLIB
AC_CHECK_PROGS([BASH], [bash sh])

SLC_PATH_LOCAL([../slc])
AC_WITH_SLC
AC_SLC_TESTS

AC_ARG_ENABLE([build-programs],
[AC_HELP_STRING([--disable-build-programs], [do not compile demonstration executables during build (default is enabled)])],
[], [enable_build_programs=yes])
AM_CONDITIONAL([ENABLE_DEMOS], [test "x$enable_build_programs" = "xyes"])

AC_CONFIG_FILES([
	Makefile
	demo/Makefile
	benchmarks/Makefile
	benchmarks/lib/Makefile
	benchmarks/blas/Makefile
	benchmarks/mandel/Makefile
	benchmarks/fft/Makefile
	benchmarks/quicksort/Makefile
	benchmarks/nas/Makefile
	benchmarks/nas/is/Makefile
	benchmarks/sha/Makefile
	benchmarks/livermore/Makefile
	benchmarks/smooth/Makefile
        benchmarks/ca/Makefile
])

AC_OUTPUT

