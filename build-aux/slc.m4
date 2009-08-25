AC_PREREQ([2.60])
m4_pattern_forbid([^_?SLC_])

# SLC_PATH_LOCAL([RELATIVE-PATH-TO-SLC-SOURCES])
# -----------------------------------------------------
# Detect SLC sources "near" the current source directory.
#
# Used in the SL-ALL distribution to instruct "configure" to use the
# bundled SLC sources.
#
# User projects can use this macro to point their "configure" to a
# nonstandard SLC sources location, by invoking it _before_ using
# AC_WITH_SLC.
#
AC_DEFUN([SLC_PATH_LOCAL],
[m4_if([$1], [], [slc_cv_local_src=no], [dnl
    AC_CACHE_CHECK([for SLC sources in local distribution],
                   [slc_cv_local_src],
                   [slc_cv_local_src=no
                    if test -r "$srcdir/$1/slc/bin/slc.in"; then
                       slc_cv_local_src="$1"
                    fi])
])])

# SLC_PATH_USER
# --------------
# Checks for the location of SLC installation specified with the user
# with the flag --with-slc.
AC_DEFUN([SLC_PATH_USER],
[dnl
  AC_CACHE_CHECK([for SLC in user-specified directory],
                 [slc_cv_user_hint],
                 [slc_cv_user_hint=no
                  AC_ARG_WITH([slc],
                     [AC_HELP_STRING([--with-slc=DIR],
                     [Base directory where SLC is installed (optional)])],
                     [if test -r "$withval/bin/slc"; then
                        slc_cv_user_hint=$withval
                      fi])])
])# SLC_PATH_USER

# _SLC_WITH_CROSS_GCC
# --------------
# Try to find the GCC cross-compilers for alpha-linux and leon-linux.

AC_DEFUN([_SLC_WITH_CROSS_GCC],
[dnl
 AC_ARG_VAR([CC_ALPHA], [Location of the GCC/Alpha cross-compiler.])
 AC_PATH_PROG([CC_ALPHA], [alpha-linux-gnu-gcc], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

 AC_ARG_VAR([CC_SPARC], [Location of the GCC/Sparc cross-compiler.])
 AC_PATH_PROG([CC_SPARC], [sparc-leon-linux-gcc], [no], [$prefix/bin$PATH_SEPARATOR$PATH])
])

# SLC_CHECK_TARGETS
# --------------
# Try to find the GCC cross-compilers for alpha-linux and leon-linux.
AC_DEFUN([SLC_CHECK_TARGETS],
[dnl
  AC_REQUIRE([AC_PROG_CXX])
  AC_REQUIRE([AC_WITH_MG_BINUTILS])
  AC_REQUIRE([AC_WITH_MG_GCC])
  AC_REQUIRE([AC_WITH_MGSIM])
  AC_REQUIRE([_SLC_WITH_CROSS_GCC])

  AC_ARG_ENABLE([ptl],
                [AC_HELP_STRING([--disable-ptl],
                                [disable support for muTC-ptl compilation (default is try to enable)])],
                [], [enable_ptl=yes])
  if test "x$enable_ptl" = "xyes"; then
    if test "x$GXX" != "xyes"; then
       AC_MSG_WARN([not using the GNU C++ compiler. muTC-ptl support is disabled.])
       enable_ptl=no
    fi
  fi
  AM_CONDITIONAL([ENABLE_SLC_PTL], [test "x$enable_ptl" = "xyes"])

  AC_ARG_ENABLE([mtalpha], 
                [AC_HELP_STRING([--disable-mtalpha], 
                                [disable support for MT-Alpha compilation (default is try to enable)])],
                [], [enable_mtalpha=yes])

  if test "x$enable_mtalpha" = "xyes"; then
    if test "x$UTCC_MTALPHA" = "xno" \
       -o "x$CC_ALPHA" = "xno" \
       -o "x$AR_MTALPHA" = "xno" \
       -o "x$RANLIB_MTALPHA" = "xno"; then
     AC_MSG_WARN([Some Alpha/MT-Alpha utilities are missing; MT-Alpha support disabled.])
     enable_mtalpha=no
    fi
  fi
  AM_CONDITIONAL([ENABLE_SLC_MTALPHA], [test "x$enable_mtalpha" = xyes])

  AC_ARG_ENABLE([mtsparc], 
                [AC_HELP_STRING([--disable-mtsparc], 
                                [disable support for MT-Sparc compilation (default is try to enable)])],
                [], [enable_mtsparc=yes])

  if test "x$enable_mtsparc" = "xyes"; then
    if test "x$UTCC_MTSPARC" = "xno" \
       -o "x$CC_SPARC" = "xno" \
       -o "x$AR_MTSPARC" = "xno" \
       -o "x$RANLIB_MTSPARC" = "xno"; then
     AC_MSG_WARN([Some Sparc/MT-Sparc utilities are missing; MT-Sparc support disabled.])
     enable_mtsparc=no
    fi
  fi
  AM_CONDITIONAL([ENABLE_SLC_MTSPARC], [test "x$enable_mtsparc" = xyes])
])

AC_DEFUN([AC_WITH_SLC],
[dnl
  AC_REQUIRE([SLC_PATH_LOCAL])
  AC_REQUIRE([SLC_PATH_USER])

  # user-specified directory overrides any other definition
  if test "x$slc_cv_user_hint" != xno; then
    if test "x$SLC_BASE" != x; then
      AC_MSG_WARN([using $slc_cv_user_hint instead of $SLC_BASE])
    fi
    if test "x$slc_cv_local_src" != xno; then
      AC_MSG_WARN([using $slc_cv_user_hint instead of local sources])
    fi
    SLC_BASE=$slc_cv_user_hint
  elif test "x$slc_cv_local_src" != xno; then
    if test "x$SLC_BASE" != x; then
      AC_MSG_WARN([using $slc_cv_local_src instead of $SLC_BASE])
    fi
    SLC_INCDIR='$(abs_top_srcdir)'/$slc_cv_local_src/slc/include:'$(abs_top_builddir)'/$slc_cv_local_src/slc/include:'$(abs_top_srcdir)'/$slc_cv_local_src/lib/include:'$(abs_top_builddir)'/$slc_cv_local_src/lib/include
    SLC_LIBDIR='$(abs_top_srcdir)'/$slc_cv_local_src/slc/lib:'$(abs_top_builddir)'/$slc_cv_local_src/slc/lib:'$(abs_top_srcdir)'/$slc_cv_local_src/lib:'$(abs_top_builddir)'/$slc_cv_local_src/lib
    SLC_DATADIR='$(abs_top_srcdir)'/$slc_cv_local_src/slc/lib:'$(abs_top_builddir)'/$slc_cv_local_src/slc/lib:'$(abs_top_srcdir)'/$slc_cv_local_src/lib:'$(abs_top_builddir)'/$slc_cv_local_src/lib
    SPP='$(abs_top_srcdir)'/$slc_cv_local_src/slc/bin/spp
    SCU='$(abs_top_srcdir)'/$slc_cv_local_src/slc/bin/scu
    SAG='$(abs_top_srcdir)'/$slc_cv_local_src/slc/bin/sag
    CCE='$(abs_top_builddir)'/$slc_cv_local_src/slc/bin/cce
    SLR='$(abs_top_builddir)'/$slc_cv_local_src/slc/bin/slr
    SLT='$(abs_top_builddir)'/$slc_cv_local_src/slc/bin/slt
    SLT_MANY='$(abs_top_builddir)'/$slc_cv_local_src/slc/bin/slt-many
    CM4='$(abs_top_builddir)'/$slc_cv_local_src/slc/bin/cm4
    SLC_LOCAL='$(abs_top_builddir)'/$slc_cv_local_src/slc/bin/slc
    SLC_BASE=
  fi
  AC_ARG_VAR([SLC_BASE], [base SLC installation directory (should be autodetected)])
  if test "x$SLC_BASE" != x; then
    SLC_LIBDIR=$SLC_BASE/lib/slc
    SLC_INCDIR=$SLC_BASE/share/slc/include
    SLC_DATADIR=$SLC_BASE/share/slc
    SPP=$SLC_BASE/bin/spp
    SCU=$SLC_BASE/bin/scu
    SAG=$SLC_BASE/bin/sag
    CCE=$SLC_BASE/bin/cce
    SLR=$SLC_BASE/bin/slr
    SLT=$SLC_BASE/bin/slt
    SLT_MANY=$SLC_BASE/bin/slt-many
    CM4=$SLC_BASE/bin/cm4
    SLC_LOCAL=$SLC_BASE/bin/slc
  fi
  AC_SUBST([SLC_LIBDIR])
  AC_SUBST([SLC_INCDIR])
  AC_SUBST([SLC_DATADIR])
  AC_SUBST([SLC_LOCAL])
  AC_SUBST([SPP])
  AC_SUBST([SCU])
  AC_SUBST([SAG])
  AC_SUBST([CCE])
  AC_SUBST([SLR])
  AC_SUBST([SLT])
  AC_SUBST([SLT_MANY])
  AC_SUBST([CM4])
])