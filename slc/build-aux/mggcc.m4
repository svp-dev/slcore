AC_DEFUN([AC_WITH_MG_GCC],
[dnl
AC_ARG_VAR([UTCC_MTALPHA], [Location of the muTC core compiler for the MT-Alpha Microgrid.])
AC_PATH_PROG([UTCC_MTALPHA], [mtalpha-linux-gnu-gcc], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

AC_ARG_VAR([UTCC_MTSPARC], [Location of the muTC core compiler for the MT-Sparc Microgrid.])
AC_PATH_PROG([UTCC_MTSPARC], [mtsparc-leon-linux-gcc], [no], [$prefix/bin$PATH_SEPARATOR$PATH])
])
