# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 

AC_PREREQ([2.61])
AC_INIT([slc], [2.x], [r.c.poss@uva.nl])
AC_CONFIG_AUX_DIR([build-aux])
AM_MAINTAINER_MODE

AM_INIT_AUTOMAKE([1.10 tar-ustar no-define foreign dist-bzip2])

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

AC_ARG_ENABLE([testsuite],
[AC_HELP_STRING([--enable-testsuite], [activate unit tests for features and regressions (default is disabled)])], 
[], [enable_testsuite=no])
AM_CONDITIONAL([ENABLE_TESTSUITE], [test "x$enable_testsuite" = "xyes"])

AC_ARG_ENABLE([xfail-tests],
[AC_HELP_STRING([--enable-xfail-tests], [activate unit tests for error detection (default is disabled)])], 
[], [enable_xfail_tests=no])
AM_CONDITIONAL([ENABLE_XFAIL_TESTS], [test "x$enable_xfail_tests" = "xyes"])

AC_ARG_ENABLE([check-utc],
[AC_HELP_STRING([--enable-check-utc], [check muTC core compiler (default is to not check)])],
[], [enable_check_utc=no])
AM_CONDITIONAL([ENABLE_CHECK_UTC], [test "x$enable_check_utc" = "xyes"])

AC_ARG_ENABLE([check-ppp],
[AC_HELP_STRING([--disable-check-ppp], [do not check PPP compiler (default is to check)])],
[], [enable_check_ppp=yes])
AM_CONDITIONAL([ENABLE_CHECK_PPP], [test "x$enable_check_ppp" = "xyes"])

AC_ARG_ENABLE([check-ptl],
[AC_HELP_STRING([--disable-check-ptl], [do not check muTC-ptl (default is to check)])],
[], [enable_check_ptl=yes])
AM_CONDITIONAL([ENABLE_CHECK_PTL], [test "x$enable_check_ptl" = "xyes"])

######### Required features #########

AC_CACHE_CHECK([for Bourne-Again shell], [ac_cv_path_BASH],
	[AC_PATH_PROGS_FEATURE_CHECK([BASH], [bash3 bash sh],
	[[$ac_path_BASH -c 'test "x$BASH_VERSION" != "x"' \
             && ac_cv_path_BASH=$ac_path_BASH ac_path_BASH_found=:]],
	[AC_MSG_ERROR([could not find bash])])])
AC_SUBST([BASH], [$ac_cv_path_BASH])

AC_PROG_GNU_M4
AM_PATH_PYTHON([2.5],[],[AC_MSG_ERROR([`slr' requires Python 2.5 or later])])



AC_ARG_ENABLE([mtalpha], 
[AC_HELP_STRING([--disable-mtalpha], [disable support for MT-Alpha compilation (default is try to enable)])],
[], [enable_mtalpha=yes])

AC_ARG_VAR([UTCC_MTALPHA], [Location of the muTC core compiler for the MT-Alpha Microgrid.])
AC_PATH_PROG([UTCC_MTALPHA], [mtalpha-linux-gnu-gcc], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

AC_ARG_VAR([CC_ALPHA], [Location of the GCC/Alpha cross-compiler.])
AC_PATH_PROG([CC_ALPHA], [alpha-linux-gnu-gcc], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

AC_ARG_VAR([AR_MTALPHA], [Location of the `ar' utility configured for the MT-Alpha binary format.])
AC_PATH_PROG([AR_MTALPHA], [mtalpha-linux-gnu-ar], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

AC_ARG_VAR([RANLIB_MTALPHA], [Location of the `ranlib' utility configured for the MT-Alpha binary format.])
AC_PATH_PROG([RANLIB_MTALPHA], [mtalpha-linux-gnu-ranlib], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

AC_ARG_VAR([MGSIM_ALPHA], [Location of the MT-Alpha Microgrid simulator.])
AC_PATH_PROG([MGSIM_ALPHA], [mgsim-alpha MGAlpha], [no], [$prefix/bin:$PATH_SEPARATOR:$PATH])

if test "x$enable_mtalpha" = "xyes"; then
  if test "x$PYTHON" = xno; then
     AC_MSG_WARN([no suitable Python found. `slc' will not produce MT-Alpha binaries.])
     enable_mtalpha=no
  elif test "x$UTCC_MTALPHA" = "xno" \
       -o "x$CC_ALPHA" = "xno" \
       -o "x$AR_MTALPHA" = "xno" \
       -o "x$RANLIB_MTALPHA" = "xno"; then
     AC_MSG_WARN([Some Alpha/MT-Alpha utilities are missing. `slc' may not produce MT-Alpha binaries.])
     enable_mtalpha=no
  fi
fi
AM_CONDITIONAL([ENABLE_MTALPHA], [test "x$enable_mtalpha" = xyes])

if test "x$MGSIM_ALPHA" = "xno"; then
   AC_MSG_WARN([Cannot find the MT-Alpha Microgrid simulator. `slr' will not run MT-Alpha programs.])
fi

AC_ARG_ENABLE([mtsparc], 
[AC_HELP_STRING([--disable-mtsparc], [disable support for MT-Sparc compilation (default is try to enable)])],
[], [enable_mtsparc=yes])

AC_ARG_VAR([UTCC_MTSPARC], [Location of the muTC core compiler for the MT-Sparc Microgrid.])
AC_PATH_PROG([UTCC_MTSPARC], [mtsparc-leon-linux-gcc], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

AC_ARG_VAR([CC_SPARC], [Location of the GCC/Sparc cross-compiler.])
AC_PATH_PROG([CC_SPARC], [sparc-leon-linux-gcc], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

AC_ARG_VAR([AR_MTSPARC], [Location of the `ar' utility configured for the MT-Sparc binary format.])
AC_PATH_PROG([AR_MTSPARC], [mtsparc-leon-linux-ar], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

AC_ARG_VAR([RANLIB_MTSPARC], [Location of the `ranlib' utility configured for the MT-Sparc binary format.])
AC_PATH_PROG([RANLIB_MTSPARC], [mtsparc-leon-linux-ranlib], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

AC_ARG_VAR([MGSIM_SPARC], [Location of the MT-Sparc Microgrid simulator.])
AC_PATH_PROG([MGSIM_SPARC], [mgsim-sparc MGSparc], [no], [$prefix/bin:$PATH_SEPARATOR:$PATH])

if test "x$enable_mtsparc" = "xyes"; then
  if test "x$PYTHON" = xno; then
     AC_MSG_WARN([no suitable Python found. `slc' will not produce MT-Sparc binaries.])
     enable_mtsparc=no
  elif test "x$UTCC_MTSPARC" = "xno" \
       -o "x$CC_SPARC" = "xno" \
       -o "x$AR_MTSPARC" = "xno" \
       -o "x$RANLIB_MTSPARC" = "xno"; then
     AC_MSG_WARN([Some Sparc/MT-Sparc utilities are missing. `slc' will not produce MT-Sparc binaries.])
     enable_mtsparc=no
  fi
fi
AM_CONDITIONAL([ENABLE_MTSPARC], [test "x$enable_mtsparc" = xyes])

if test "x$MGSIM_SPARC" = "xno"; then
   AC_MSG_WARN([Cannot find the MT-Sparc Microgrid simulator. `slr' may not run MT-Sparc programs.])
fi

AC_CONFIG_FILES([
	Makefile
	bin/Makefile
        lib/r-mtalpha-sim/run.sh
        lib/r-mtsparc-sim/run.sh
	lib/Makefile
	doc/Makefile
	tests/Makefile
	demo/Makefile
])
AC_OUTPUT

