// SLT_RUN:  M=3 -dPi  Pf=1 -dPc  BR=1
// SLT_RUN:  M=4 -dPi  Pf=1 -dPc  BR=1
// SLT_RUN:  M=4 -dPi  Pf=1 -dPc  BR=1 -Ws,-o -Ws,NumProcessors=3

// FIXME: as of 2004-04-05 the output of this
// program seems implementation dependent;
// so ignore the diff during testing:
// XIGNORE: *:D
m4_include(fft_impl1.sl)
m4_include(fft_extra.sl)
m4_include(fft_test.sl)

