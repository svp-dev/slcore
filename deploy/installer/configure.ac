AC_PREREQ([2.65])
AC_INIT([sl-installer], 
        m4_esyscmd([(pre=; if test -d ../../.git -a ! -d .git; then pre=$PWD/; cd ../..; fi; \
                    ${pre}build-aux/version-gen \
                    ${pre}build-aux/tarball-version \
                    ${pre}build-aux/package-version)]), 
        [sl-users@nic.surfnet.nl])
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

m4_include([versions.m4])

m4_define([handle_package],[
AC_ARG_WITH([$1-version], 
            [AC_HELP_STRING([--with-$1-version=VVV], 
                            [use version VVV for $1 (default: ]def_$1_ver[)])],
            [], [with_$1_version=def_$1_ver])
AC_ARG_WITH([$1-archive],
            [AC_HELP_STRING([--with-$1-archive=ARCH],
                            [use file ARCH for $1 (default: distfiles/]m4_ifblank([$2],[$1],[$2])[-VVV.tar.bz2)])],
            [], [with_$1_archive=distfiles/m4_ifblank([$2],[$1],[$2])-$with_$1_version.tar.bz2])

the_dir=`dirname "$with_$1_archive"`
mkdir -p "$the_dir" || true
the_dir=`(cd "$the_dir" && pwd)`
the_name=$the_dir/`basename "$with_$1_archive"`
the_name=`echo "$the_name" | sed -e 's|^'"$PWD"'/||'`
with_$1_archive=$the_name

m4_toupper($1)_VERSION=$with_$1_version
m4_toupper($1)_ARCHIVE=$with_$1_archive
AC_SUBST(m4_toupper($1)_VERSION)
AC_SUBST(m4_toupper($1)_ARCHIVE)
])

handle_package([binutils])
handle_package([mggcc])
handle_package([gcc])
handle_package([mgsim])
handle_package([slc], [sl-core])
handle_package([sc], [systemc])
handle_package([m4])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "*"
echo "* Configuration done."
echo "*"
echo "* Destination dir:   $prefix"
echo "* Remote repository: $DIST_SITE"
echo "* URL fetch cmd:     $FETCH"
echo "* GNU make:          $MAKE"
echo "* C compiler:        $CC $CPPFLAGS $CFLAGS $LDFLAGS"
echo "* C++ compiler:      $CXX $CPPFLAGS $CXXFLAGS $LDFLAGS"
echo "*"
echo "* mg-binutils:       $BINUTILS_ARCHIVE ($BINUTILS_VERSION)"
echo "* uTC core compiler: $MGGCC_ARCHIVE ($MGGCC_VERSION)"
echo "* GCC (patched):     $GCC_ARCHIVE ($GCC_VERSION)"
echo "* m4 (patched):      $M4_ARCHIVE ($M4_VERSION)"
echo "* SystemC (patched): $SC_ARCHIVE ($SC_VERSION)"
echo "* mgsim:             $MGSIM_ARCHIVE ($MGSIM_VERSION)"
echo "* slc:               $SLC_ARCHIVE ($SLC_VERSION)"
echo "*"
echo "Now run:"
echo 
echo "    $MAKE install"
echo "or"
echo "    $MAKE help"
echo 
