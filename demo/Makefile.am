DEMOS = 10threads.sl answer.sl fibo.sl helloworld.sl test.sl

check_DATA = $(DEMOS:.sl=.ptl-x) $(DEMOS:.sl=.seqc-x)
EXTRA_DIST = $(DEMOS)
CLEANFILES = $(check_DATA)

SLC = $(top_builddir)/bin/slc.local
SUFFIXES = .sl .seqc-x .ptl-x

.sl.seqc-x:
	$(SLC) -b seqc -o $@ $<

.sl.ptl-x:
	$(SLC) -b ptl -o $@ $<

