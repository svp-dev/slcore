AC_DEFUN([AC_SLC_TESTS],
[dnl
AC_REQUIRE([SLC_CHECK_TARGETS])

AC_ARG_ENABLE([testsuite],
[AC_HELP_STRING([--enable-testsuite], [activate unit tests for features and regressions (default is enabled)])], 
[], [enable_testsuite=yes])
AM_CONDITIONAL([ENABLE_TESTSUITE], [test "x$enable_testsuite" = "xyes"])

AC_ARG_ENABLE([xfail-tests],
[AC_HELP_STRING([--enable-xfail-tests], [activate unit tests for error detection (default is disabled)])], 
[], [enable_xfail_tests=no])
AM_CONDITIONAL([ENABLE_XFAIL_TESTS], [test "x$enable_xfail_tests" = "xyes"])

AC_ARG_ENABLE([check-utc],
[AC_HELP_STRING([--enable-check-utc], [check muTC core compiler (default is to not check)])],
[enable_check_utc=no
 if test "x$enableval" = xyes; then
    enable_check_utc=$enable_mtalpha
 fi], [enable_check_utc=no])
AM_CONDITIONAL([ENABLE_CHECK_UTC], [test "x$enable_check_utc" = "xyes"])

AC_ARG_ENABLE([check-ppp],
[AC_HELP_STRING([--disable-check-ppp], [do not check PPP compiler (default is to check)])],
[enable_check_ppp=no
 if test "x$enableval" = xyes; then
   enable_check_ppp=$enable_mtalpha
 fi], [enable_check_ppp=$enable_mtalpha])
AM_CONDITIONAL([ENABLE_CHECK_PPP], [test "x$enable_check_ppp" = "xyes"])

AC_ARG_ENABLE([check-ptl],
[AC_HELP_STRING([--disable-check-ptl], [do not check muTC-ptl (default is to check)])],
[enable_check_ptl=no
 if test "x$enableval" = xyes; then
   enable_check_ptl=$enable_ptl
 fi], [enable_check_ptl=$enable_ptl])
AM_CONDITIONAL([ENABLE_CHECK_PTL], [test "x$enable_check_ptl" = "xyes"])
])