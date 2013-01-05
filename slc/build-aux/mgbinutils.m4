AC_DEFUN([AC_WITH_MG_BINUTILS],
[dnl
 AC_ARG_VAR([AR_MTALPHA], [Location of the `ar' utility configured for the MT-Alpha binary format.])
 AC_PATH_PROG([AR_MTALPHA], [mtalpha-linux-gnu-ar], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

 AC_ARG_VAR([RANLIB_MTALPHA], [Location of the `ranlib' utility configured for the MT-Alpha binary format.])
 AC_PATH_PROG([RANLIB_MTALPHA], [mtalpha-linux-gnu-ranlib], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

 AC_ARG_VAR([AR_MTSPARC], [Location of the `ar' utility configured for the MT-Sparc binary format.])
 AC_PATH_PROG([AR_MTSPARC], [mtsparc-linux-gnu-ar], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

 AC_ARG_VAR([RANLIB_MTSPARC], [Location of the `ranlib' utility configured for the MT-Sparc binary format.])
 AC_PATH_PROG([RANLIB_MTSPARC], [mtsparc-linux-gnu-ranlib], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

 AC_ARG_VAR([AR_MIPSEL], [Location of the `ar' utility configured for the MIPSel binary format.])
 AC_PATH_PROG([AR_MIPSEL], [mipsel-linux-gnu-ar], [no], [$prefix/bin$PATH_SEPARATOR$PATH])

 AC_ARG_VAR([RANLIB_MIPSEL], [Location of the `ranlib' utility configured for the MIPSel binary format.])
 AC_PATH_PROG([RANLIB_MIPSEL], [mipsel-linux-gnu-ranlib], [no], [$prefix/bin$PATH_SEPARATOR$PATH])
])
