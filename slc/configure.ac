## configure.ac: this file is part of the SL toolchain.
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
AC_INIT([sl-core], 
        m4_esyscmd([(pre=; if test -d ../.git -a ! -d .git; then pre=$PWD/; cd ..; fi; \
                    ${pre}build-aux/version-gen ${pre}build-aux/tarball-version ${pre}build-aux/package-version)]), 
        [sl-users@nic.surfnet.nl])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 tar-ustar no-define foreign dist-bzip2 silent-rules])

AM_SILENT_RULES([yes])

######### Required features #########

AC_CACHE_CHECK([for Bourne-Again shell], [ac_cv_path_BASH],
	[AC_PATH_PROGS_FEATURE_CHECK([BASH], [bash3 bash sh],
	[[$ac_path_BASH -c 'test "x$BASH_VERSION" != "x"' \
             && ac_cv_path_BASH=$ac_path_BASH ac_path_BASH_found=:]],
	[AC_MSG_ERROR([could not find bash])])])
AC_SUBST([BASH], [$ac_cv_path_BASH])

AC_PROG_GNU_M4
AM_PATH_PYTHON([2.5],[],[AC_MSG_ERROR([`slr' requires Python 2.5 or later])])


m4_define([SLC_OPT_PROG],
[AC_ARG_VAR(m4_toupper([$1]), [$2])
AC_CHECK_PROGS(m4_toupper([$1]), [m4_default([$3], [$1])],
               [$am_aux_dir/missing $1])
])

SLC_OPT_PROG([perl],
	  [Perl interpreter],
	  [perl])
SLC_OPT_PROG([rst2latex],
          [reStructuredText to LaTeX converter],
          [rst2latex rst2latex.py])
SLC_OPT_PROG([rst2html],
          [reStructuredText to HTML converter],
          [rst2html rst2html.py])
SLC_OPT_PROG([texi2pdf],
          [Texinfo/TeX to PDF converter],
          [texi2pdf])
SLC_OPT_PROG([help2man],
          [--help output to manpage converter],
          [help2man])

AC_PROG_EGREP
AC_PROG_SED
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_LN_S
AX_PTHREAD

######### Options #########

SLC_PATH_LOCAL([.])
AC_WITH_SLC
SLC_CHECK_TARGETS

if test "x$MGSIM_ALPHA" = "xno"; then
   AC_MSG_WARN([Cannot find the MT-Alpha Microgrid simulator. `slr' will not run MT-Alpha programs.])
fi

if test "x$MGSIM_SPARC" = "xno"; then
   AC_MSG_WARN([Cannot find the MT-Sparc Microgrid simulator. `slr' may not run MT-Sparc programs.])
fi

AC_CONFIG_FILES([
	Makefile
	build-aux/Makefile
        tools/Makefile
	tools/bin/Makefile
	tools/lib/Makefile
        tools/lib/slc_config.py
	lib/Makefile
	doc/Makefile
])

AC_OUTPUT

