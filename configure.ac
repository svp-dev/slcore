AC_PREREQ([2.63])
AC_INIT([slc], [2.x], [r.c.poss@uva.nl])
AC_CONFIG_AUX_DIR([build-aux])
AM_MAINTAINER_MODE

AM_INIT_AUTOMAKE([1.10 tar-ustar no-define foreign dist-bzip2])

m4_define([SLC_PROGS],
[AC_ARG_VAR(m4_toupper([$1]), [$2])
AC_CHECK_PROGS(m4_toupper([$1]), [m4_default([$3], [$1])],
               [$am_aux_dir/missing $1])
])

SLC_PROGS([perl],
	  [Perl interpreter],
	  [perl])
SLC_PROGS([rst2latex],
          [reStructuredText to LaTeX converter],
          [rst2latex rst2latex.py])
SLC_PROGS([rst2html],
          [reStructuredText to HTML converter],
          [rst2html rst2html.py])
SLC_PROGS([texi2pdf],
          [Texinfo/TeX to PDF converter],
          [texi2pdf])
SLC_PROGS([help2man],
          [--help output to manpage converter],
          [help2man])
SLC_PROGS([m4],
	  [GNU M4 preprocessor],
	  [gm4 m4])

AC_CACHE_CHECK([for Bourne-Again shell], [ac_cv_path_SBASH],
		    [AC_PATH_PROGS_FEATURE_CHECK([SBASH], [bash3 bash sh],
		    [[$ac_path_SBASH -c 'test "x$BASH_VERSION" != "x"' && ac_cv_path_SBASH=$ac_path_SBASH ac_path_SBASH_found=:]],
		    [AC_MSG_ERROR([could not find bash])])])
AC_SUBST([SBASH], [$ac_cv_path_SBASH])

SLC_PROGS([mtalpha_ar],
	  [AR utility for mtalpha binaries],
	  [mtalpha-ar mtalpha-linux-gnu-ar])
SLC_PROGS([mtalpha_ranlib],
	  [RANLIB utility for mtalpha binaries],
	  [mtalpha-ranlib mtalpha-linux-gnu-ranlib])

AC_PROG_EGREP
AC_PROG_SED
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

AM_PATH_PYTHON

AC_CONFIG_FILES([
	Makefile
	bin/Makefile
	lib/Makefile
	doc/Makefile
	demo/Makefile
])
AC_OUTPUT
