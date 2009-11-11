
AC_PREREQ([2.61])
AC_INIT([tinyxml], [2.5.3], [sl-users@nic.surfnet.nl])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 tar-ustar no-define foreign dist-bzip2])

AC_PROG_CXX
AC_PROG_RANLIB
CXXFLAGS='-DTIXML_USE_STL'

AC_ARG_ENABLE([profile], 
	      [AC_HELP_STRING([--enable-profile], 
		              [compile with profiling enabled (default is disabled)])],
	      [], [enable_profile=no])
if test "x$enable_profile" = "xyes"; then
   CXXFLAGS="$CXXFLAGS -pg"
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT