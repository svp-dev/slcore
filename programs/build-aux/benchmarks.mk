include $(top_srcdir)/build-aux/sl.mk
SLFLAGS = -I$(top_srcdir)/benchmarks/lib -lbench
SLFLAGS_PTL = -L$(top_builddir)/benchmarks/lib/host-host-ptl
SLFLAGS_SEQC = -L$(top_builddir)/benchmarks/lib/host-host-seqc
SLFLAGS_MTALPHA = -L$(top_builddir)/benchmarks/lib/mtalpha-sim

if ENABLE_DEMOS
noinst_DATA = $(BENCHMARKS:.c=.x)
CLEANFILES = $(BENCHMARKS:.c=.x) $(BENCHMARKS:.c=.bin.*)
endif

PLIST ?= 1 2 4 8 16 32 64
BENCH_BINFORMATS ?= seqc ptl mtalpha

bench.mk: $(top_srcdir)/build-aux/genbench.sh Makefile.am $(BENCHMARK_INPUTS)
	$(AM_V_at)rm -f $@ $@.tmp
	$(AM_V_GEN)PLIST="$(PLIST)" BENCH_BINFORMATS="$(BENCH_BINFORMATS)" \
	   $(BASH) $(top_srcdir)/build-aux/genbench.sh $(srcdir) $(top_builddir) $(BENCHMARKS) >$@.tmp
	$(AM_V_at)chmod -w $@.tmp
	$(AM_V_at)mv $@.tmp $@

.PHONY: bench clean-local-bench distclean-local-bench ub-archives

export AM_DEFAULT_VERBOSITY

bench: $(BENCHMARKS:.c=.x) bench.mk
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -f bench.mk results

ub-archives: $(BENCHMARKS:.c=.tar)

SUFFIXES += .tar
BENCHLIB = $(abs_top_srcdir)/benchmarks/lib/benchmark.c \
           $(abs_top_srcdir)/benchmarks/lib/benchmark.h


.c.tar: $(EXTRA_DIST) $(BUILT_SOURCES) $(BENCHLIB)
	$(AM_V_at)rm -rf $@ $@.tmp
	$(AM_V_GEN)b="$<" && \
	  bn=$$(basename $$b .c) && \
	  rm -rf "$$bn" && \
	  mkdir "$$bn" && \
	  for f in $(EXTRA_DIST) $(BUILT_SOURCES) $(BENCHLIB); do \
	    dn=$$(dirname $$(echo $$f|$(SED) -e 's|^'$(abs_top_srcdir).*/'||g')) && \
	    mkdir -p $$bn/$$dn && \
	    cp `test -r $$f || echo $(srcdir)/`$$f $$bn/$$dn/; \
	  done && \
	  echo "a.out: $$bn.c benchmark.c; "'$$'"(COMPILER) "'$$'"(FLAGS) -I. -o "'$$'"@ "'$$'"^" \
	     >$$bn/Makefile && \
	  tardir=$$bn && $(am__tar) >$@.tmp && \
	  rm -rf $$bn
	$(AM_V_at)chmod -w $@.tmp
	$(AM_V_at)mv -f $@.tmp $@

clean-local-bench:
	if test -r bench.mk; then $(MAKE) $(AM_MAKEFLAGS) -f bench.mk clean; fi

distclean-local-bench:
	rm -rf bench.mk benchdata

clean-local: clean-local-bench
distclean-local: distclean-local-bench

