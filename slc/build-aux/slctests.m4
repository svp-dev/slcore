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

AC_ARG_ENABLE([check-ptl],
[AC_HELP_STRING([--disable-check-ptl], [do not check muTC-ptl (default is to check)])],
[enable_check_ptl=no
 if test "x$enableval" = xyes; then
   enable_check_ptl=$enable_ptl
 fi], [enable_check_ptl=$enable_ptl])
AM_CONDITIONAL([ENABLE_CHECK_PTL], [test "x$enable_check_ptl" = "xyes"])

AC_ARG_ENABLE([check-hlsim],
[AC_HELP_STRING([--disable-check-hlsim], [do not check hlsim (default is to check)])],
[enable_check_hlsim=no
 if test "x$enableval" = xyes; then
   enable_check_hlsim=$enable_hlsim
 fi], [enable_check_hlsim=$enable_hlsim])
AM_CONDITIONAL([ENABLE_CHECK_HLSIM], [test "x$enable_check_hlsim" = "xyes"])

AC_ARG_ENABLE([check-mt],
[AC_HELP_STRING([--disable-check-mt], [do not check MT compiler (default is to check)])],
[enable_check_mt=$enableval], [enable_check_mt=yes])

if test "x$enable_check_mt" != xyes; then
  enable_mtalpha=no
  enable_mtsparc=no
  enable_mipsel=no
fi
AM_CONDITIONAL([ENABLE_CHECK_MT], [test "x$enable_check_mt" = "xyes"])

AM_CONDITIONAL([ENABLE_CHECK_MTA], [test "x$enable_mtalpha" = "xyes"])
AM_CONDITIONAL([ENABLE_CHECK_MTS], [test "x$enable_mtsparc" = "xyes"])
AM_CONDITIONAL([ENABLE_CHECK_MIPSEL], [test "x$enable_mipsel" = "xyes"])

])
