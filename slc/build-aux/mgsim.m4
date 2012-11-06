AC_DEFUN([AC_WITH_MGSIM],
[dnl
  AC_ARG_VAR([MGSIM_ALPHA], [Location of the Alpha-based Microgrid simulator.])
  AC_PATH_PROGS([MGSIM_ALPHA], [mtalpha-mgsim mgsim-alpha MGAlpha], [no], 
               [$prefix/bin$PATH_SEPARATOR$PATH])

  AC_ARG_VAR([MGSIM_SPARC], [Location of the SPARC-based Microgrid simulator.])
  AC_PATH_PROGS([MGSIM_SPARC], [mtsparc-mgsim mgsim-sparc MGSparc], [no], 
               [$prefix/bin$PATH_SEPARATOR$PATH])

  AC_ARG_VAR([MGSIM_MIPSEL], [Location of the MIPSel-based Microgrid simulator.])
  AC_PATH_PROGS([MGSIM_MIPSEL], [mipsel-mgsim mgsim-mipsel], [no], 
               [$prefix/bin$PATH_SEPARATOR$PATH])
])
