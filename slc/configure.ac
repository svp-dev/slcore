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

AC_PREREQ([2.65])
AC_INIT([sl-core], 
        m4_esyscmd([(pre=; if test -d ../.git -a ! -d .git; then pre=$PWD/; cd ..; fi; \
                    ${pre}build-aux/version-gen ${pre}build-aux/tarball-version ${pre}build-aux/package-version)]), 
        [sl-users@svp-home.org])
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
AM_PATH_PYTHON([2.6],[],[AC_MSG_ERROR([The SL tool chain requires Python >= 2.6.])])


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
AC_PROG_CC_C99

if test "x$ac_cv_prog_cc_c99" = "xno"; then
   AC_MSG_ERROR([Cannot find a C99 compiler.])
fi

AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_LN_S
AX_PTHREAD
AX_TLS

######### Options #########

SLC_PATH_LOCAL([.])
AC_WITH_SLC
SLC_CHECK_TARGETS

if test "x$ax_pthread_ok" = "xyes" -a "x$ac_cv_tls" != "xnone"; then
   PTHREAD_CFLAGS="$PTHREAD_CFLAGS -DTHREADSPEC=$ac_cv_tls"
fi
AM_CONDITIONAL([CHECK_TLSMALLOC], [test "x$ax_pthread_ok" = "xyes" -a "x$ac_cv_tls" != "xnone"])


if test "x$enable_ptl" = "xyes"; then
  if test "x$GXX" != "xyes"; then
     AC_MSG_WARN([not using the GNU C++ compiler. muTC-ptl support is disabled.])
     enable_ptl=no
  fi
  if test "x$ax_pthread_ok" != "xyes"; then
     AC_MSG_WARN([POSIX threads not available. muTC-ptl support is disabled.])
     enable_hrt=no
  fi
fi

if test "x$enable_hrt" = "xyes"; then
  if test "x$ax_pthread_ok" != "xyes"; then
     AC_MSG_WARN([POSIX threads not available. hrt support is disabled.])
     enable_hrt=no
  fi
fi

if test "x$enable_mtalpha" = "xyes"; then
  if test "x$UTCC_MTALPHA" = "xno" \
     -o "x$CC_ALPHA" = "xno" \
     -o "x$AR_MTALPHA" = "xno" \
     -o "x$RANLIB_MTALPHA" = "xno"; then
   AC_MSG_WARN([Some Alpha/MT-Alpha utilities are missing; MT-Alpha support disabled.])
   enable_mtalpha=no
  fi
fi

if test "x$enable_mtsparc" = "xyes"; then
  if test "x$UTCC_MTSPARC" = "xno" \
     -o "x$CC_SPARC" = "xno" \
     -o "x$AR_MTSPARC" = "xno" \
     -o "x$RANLIB_MTSPARC" = "xno"; then
   AC_MSG_WARN([Some Sparc/MT-Sparc utilities are missing; MT-Sparc support disabled.])
   enable_mtsparc=no
  fi
fi

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
        lib/src/tlsmalloc/Makefile
	doc/Makefile
])

AC_OUTPUT

