DEMOS = 10threads.sl answer.sl fibonacci.sl helloworld.sl # test.sl

check_DATA = $(DEMOS:.sl=.ptl-x) $(DEMOS:.sl=.seqc-x) $(DEMOS:.sl=.mtalpha-x)
EXTRA_DIST = $(DEMOS)
CLEANFILES = $(check_DATA)

SLC = $(top_builddir)/bin/slc.local
SUFFIXES = .sl .seqc-x .ptl-x .mtalpha-x

.sl.seqc-x:
	$(SLC) -b seqc -o $@ $<

.sl.ptl-x:
	$(SLC) -b ptl -o $@ $<

.sl.mtalpha-x:
	$(SLC) -b ppp-mtalpha -o $@ $<

