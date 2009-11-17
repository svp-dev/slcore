AC_DEFUN([AC_WITH_MGSIM],
[dnl
  AC_ARG_VAR([MGSIM_ALPHA], [Location of the Alpha-based Microgrid simulator.])
  AC_PATH_PROG([MGSIM_ALPHA], [mgsim-alpha MGAlpha], [no], 
               [$prefix/bin$PATH_SEPARATOR$PATH])

  AC_ARG_VAR([MGSIM_SPARC], [Location of the SPARC-based Microgrid simulator.])
  AC_PATH_PROG([MGSIM_SPARC], [mgsim-sparc MGSparc], [no], 
               [$prefix/bin$PATH_SEPARATOR$PATH])

  AC_ARG_VAR([SIMX_ALPHA], 
             [Location of the Alpha-based Microgrid simulator with COMA.])
  AC_PATH_PROG([SIMX_ALPHA], [simx-alpha], [no], 
               [$prefix/bin$PATH_SEPARATOR$PATH])

  AC_ARG_VAR([SIMX_SPARC], 
             [Location of the SPARC-based Microgrid simulator with COMA.])
  AC_PATH_PROG([SIMX_SPARC], [simx-sparc MGSparc], [no], 
               [$prefix/bin$PATH_SEPARATOR$PATH])
])
