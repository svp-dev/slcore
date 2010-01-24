AC_PREREQ([2.61])
AC_INIT([sl-installer], [2.2a], [sl-users@nic.surfnet.nl])
AC_CONFIG_AUX_DIR([build-aux])

AC_PREFIX_DEFAULT([/opt/svp])

AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_PROG_LN_S
AC_PROG_GREP
AC_PROG_SED

#
# The canonical build name is needed to specialize $FETCH below.
#
AC_CANONICAL_BUILD

#
# Find a known URL retrieve program.
#
AC_ARG_VAR([FETCH], [URL retrieve command])
AC_PATH_PROGS([FETCH], [wget curl])
if test -z "$FETCH"; then
  AC_MSG_WARN([Cannot find a URL retrieve command. Archive files must be placed manually into distfiles/.])
  FETCH="${am_missing_run}wget"
fi

case $FETCH in
     *curl*) FETCH="$FETCH -O" ;;
esac

#
# If using the default URL, do not check SSL certificate.
#
AC_ARG_VAR([DIST_SITE], [base URL to retrieve source archives])
default_site=https://mac-chris.science.uva.nl/csa/dist/deploy
if test "x$DIST_SITE" = "x"; then
   DIST_SITE=$default_site
   case $FETCH in
      *wget*) FETCH="$FETCH --no-check-certificate" ;;
      *curl*) FETCH="$FETCH -k" ;;
   esac
fi

#
# Look for GNU make, before initializing Automake.
#
AC_ARG_VAR([MAKE], [path to GNU make])
AC_CACHE_CHECK([for GNU make], [ac_cv_path_MAKE],
[if ${MAKE-make} --version 2>/dev/null | $ac_path_GREP GNU >/dev/null; then
  ac_cv_path_MAKE=${MAKE-make}
else
  AC_PATH_PROGS_FEATURE_CHECK([MAKE], [make gmake gnumake],
    [[$ac_path_MAKE --version 2>/dev/null | $ac_path_GREP GNU >/dev/null && ac_cv_path_MAKE=$ac_path_MAKE ac_path_MAKE_found=:]],
    [AC_MSG_ERROR([could not find GNU make])])
fi
])
AC_SUBST([MAKE], [$ac_cv_path_MAKE])

# The following should appear *after* checking for GNU make above.
AM_INIT_AUTOMAKE([1.11 foreign dist-bzip2])


AC_PROG_CC
AC_PROG_CXX

#
# If MacPorts is installed, assume it should be used.
# Also, fish for GNU getopt on MacOS X 10.5+.
#
case $build in
  *-apple-darwin*)
  AC_MSG_CHECKING([for MacPorts])
  if test -d /opt/local/var/macports; then
    AC_MSG_RESULT([yes]) 
    CPPFLAGS="$CPPFLAGS -I/opt/local/include"
    LDFLAGS="$LDFLAGS -L/opt/local/lib"
    if test -d /opt/local/include/libgnugetopt; then
       CPPFLAGS="$CPPFLAGS -I/opt/local/include/libgnugetopt"
    fi
  else
    AC_MSG_RESULT([no])
  fi
  ;;
esac

case $build in
  *-apple-darwin[1-8].*) ;;
  *-apple-darwin*)
    AC_LANG_PUSH([C])
    AC_CHECK_LIB([gnugetopt], [getopt_long], [], [AC_MSG_WARN([GNU getopt is required with MacOS X 10.5+ (MacPorts: port install libgnugetopt)])])
    AC_LANG_POP([C])
    ;;
esac

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "*"
echo "* Configuration done."
echo "*"
echo "* Archive source:   $DIST_SITE"
echo "* Destination dir:  $prefix"
echo "* URL fetch cmd:    $FETCH"
echo "* GNU make:         $MAKE"
echo "* C compiler:       $CC $CPPFLAGS $CFLAGS $LDFLAGS"
echo "* C++ compiler:     $CXX $CPPFLAGS $CXXFLAGS $LDFLAGS"
echo "*"
echo "Now run:"
echo 
echo "    $MAKE install"
echo "or"
echo "    $MAKE help"
echo 
