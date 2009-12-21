###
### Build settings
###
include $(top_srcdir)/build-aux/sl.mk
SLFLAGS = -I$(top_srcdir)/benchmarks/lib -lbench
SLFLAGS_PTL = -L$(top_builddir)/benchmarks/lib/host-host-ptl
SLFLAGS_SEQC = -L$(top_builddir)/benchmarks/lib/host-host-seqc
SLFLAGS_MTALPHA = -L$(top_builddir)/benchmarks/lib/mtalpha-sim
CLEANFILES =
DISTCLEANFILES = 

###
### Generating dependency makefiles
###
PLIST ?= 1 2 4 8 16 32 64
BENCH_BINFORMATS ?= seqc ptl mtalpha

SUFFIXES += .ilist
ILIST_FILES = $(BENCHMARKS:.c=.ilist)

%.ilist: %.c Makefile
	$(AM_V_at)rm -f $@ $@.tmp1 $@.tmp2 $@.tmp3
	$(AM_V_GEN)set -e;							\
	        ifile=`test -r "$*".inputs || echo "$(srcdir)/"`$*.inputs; \
		if test -r "$$ifile"; then					\
		  { while read pat; do						\
		    for f in "$(srcdir)"/$$pat.d*[0-9] ./$$pat.d*[0-9]; do	\
		      if test -r "$$f"; then echo "$$f"; fi;			\
		    done;							\
		  done; } <"$$ifile";						\
		else								\
		  for f in "$(srcdir)/$*".d*[0-9] ./"$*".d*[0-9]; do		\
		     if test -r "$$f"; then echo "$$f"; fi;			\
		  done;								\
		fi >$@.tmp1 && \
		for i in `cat $@.tmp1`; do \
	           suff=; if grep -q 'USE IN MAKE CHECK' "$$i" >/dev/null 2>&1; then suff=.check; fi; \
	           echo "$$i$$suff"; \
	        done >$@.tmp2
	$(AM_V_at)sed -e 's|^$(srcdir)/||g' <$@.tmp2 | sort | uniq >$@.tmp3
	$(AM_V_at)chmod -w $@.tmp3 && mv -f $@.tmp3 $@ && rm -f $@.tmp1 $@.tmp2

.PRECIOUS: $(ILIST_FILES)
DISTCLEANFILES += $(ILIST_FILES)

##
## Fibre data files
##

SUFFIXES += .fdata

GENDATA_DEF = gen_fdata() { \
	  set -e; \
	  target=$$1; \
	  binfmt=$$2; \
	  data=`test -r "$$3" || echo "$(srcdir)/"`$$3; \
	  rm -f "$$target"; mkdir -p benchdata; \
	  TIMEOUT=$${TIMEOUT:-7200} $(TMO) $(SLR) -b $$binfmt -f "$$data" -wf "$$target".tmp -rd /dev/null -wo && \
	  mv -f "$$target".tmp "$$target"; \
	}

fdatas.mk: Makefile $(BENCHMARKS:.c=.ilist)
	$(AM_V_at)rm -f $@ $@.tmp
	$(AM_V_GEN)set -e; \
	   ifiles=`cat $(BENCHMARKS:.c=.ilist)|sort|uniq`; \
	   for i1 in $$ifiles; do \
	     i=`echo "$$i1"|sed -e 's/.check$$//g'`; \
	     ibase=`basename "$$i"`; \
	     for b in $(BENCH_BINFORMATS); do \
	       idata=benchdata/$$b-$$ibase.fdata; \
	       echo "FDATA_FILES += $$idata"; \
	       echo "$$idata: $$i ; \$$(AM_V_GEN)\$$(GENDATA_DEF); gen_fdata \$$@ $$b \$$^"; \
	     done; \
	   done >$@.tmp
	$(AM_V_at)chmod -w $@.tmp && mv -f $@.tmp $@

FDATA_FILES =
-include fdatas.mk

.PRECIOUS: $(FDATA_FILES)
.PHONY: fdata clean-fdata
fdata: $(FDATA_FILES)
clean-fdata:
	-test -z "$(FDATA_FILES)" || rm -f $(FDATA_FILES)

CLEANFILES += $(FDATA_FILES)
DISTCLEANFILES += fdata.mk

##
## Benchmarks
##

SUFFIXES += .bmk .out
BMK_FILES = $(BENCHMARKS:.c=.bmk)

FAIL_DIR = $(top_builddir)/failures

DOBENCH_DEF = do_bench() { \
	  set -e; \
	  target=$$1; \
	  binfmt=$$2; \
	  ncores=$$3; \
	  prog=`basename "$$4" .x`.bin.$$binfmt; \
	  fdata=$$5; \
	  dores=`if test $$6 = 1; then echo 1; fi`; \
	  rm -f "$$target" "$$target".err; \
	  set +e; TIMEOUT=$${TIMEOUT:-10800} $(TMO) $(SLR) "$$prog" -rf "$$fdata" \
	    L= sep_dump= results=$$dores format=1 ncores=$$ncores \
	    -b "$$binfmt" -t -p "$$target".work >>"$$target".err 2>&1; \
	  ecode=$$?; set -e; if test $$ecode != 0; then \
	    if test -n "$$dores"; then \
	      { echo "Exit status: $$ecode"; echo; echo "Error log::"; echo; sed -e 's/^/  /g' <"$$target".err; } >&2; \
	    fi; \
	    scode=`expr $$ecode - 128`; \
	    if ! test x$$scode = x1 \
	       -o x$$scode = x2 \
	       -o x$$scode = x15; then \
	         $(MKDIR_P) $(FAIL_DIR); cp -r "$$target".err "$$target".work $(FAIL_DIR); \
	    fi; \
	    exit $$ecode; \
	  fi; \
	  mv -f "$$target".err "$$target" && rm -rf "$$target".work; \
	}

%.bmk: %.ilist Makefile $(top_srcdir)/build-aux/benchmarks.mk
	$(AM_V_at)rm -f $@ $@.tmp
	$(AM_V_GEN)set -e; \
	   ilist=`cat "$*".ilist`; ilist=`for i in $$ilist; do basename $$i; done`; \
	   elist=""; \
	   for b in $(BENCH_BINFORMATS); do \
	      plist=""; \
	      for ibase1 in $$ilist; do \
	         ibase=`basename "$$ibase1" .check`; \
	         docheck=`if test "x$$ibase" != "x$$ibase1"; then echo 1; fi`; \
	         idata=benchdata/$$b-$$ibase.fdata; \
	         for p in $(PLIST); do \
	            pdata=benchdata/$$b-$*-$$ibase.p$$p.out; \
	            plist+=" $$pdata"; \
		    echo "$$pdata: $*.x $$idata ; " \
	                 "\$$(AM_V_GEN)\$$(DOBENCH_DEF); do_bench \$$@ $$b $$p \$$^ 0"; \
	         done; \
	         if test -n "$$docheck"; then \
	            edata=benchdata/$$b-$*-$$ibase.check.out; \
		    elist+=" $$edata"; \
	            echo "$$edata: $*.x $$idata ; " \
	                 "\$$(AM_V_GEN)\$$(DOBENCH_DEF); do_bench \$$@ $$b 1 \$$^ 1"; \
	         fi; \
	      done; \
	      echo "PDATA_FILES += $$plist"; \
	      echo ".PHONY: $$b-$*.gen"; \
	      echo "$$b-$*.gen: $$plist"; \
	   done >$@.tmp && \
	   { echo ".PHONY: $*.check"; \
	     echo "$*.check: $$elist ; " \
	          "cat \$$^ && rm -f \$$^"; \
	   } >>$@.tmp
	$(AM_V_at)chmod -w $@.tmp && mv -f $@.tmp $@

PDATA_FILES =
-include $(BMK_FILES)

.PRECIOUS: $(PDATA_FILES)
.PHONY: bench clean-bench
bench: $(PDATA_FILES)
clean-bench:
	-test -z "$(PDATA_FILES)" || rm -f $(PDATA_FILES)

CLEANFILES += $(PDATA_FILES)
DISTCLEANFILES += $(BMK_FILES)


clean-local:
	-rm -rf benchdata



##
## Unit testing
##

TEST_EXTENSIONS = .bmk
BMK_LOG_COMPILER = \
	docheck() { t=`basename "$$1" .bmk`.check; $(MAKE) $(AM_MAKEFLAGS) "$$t" V=1; }; $(SLC_VARS) docheck
TESTS = $(BENCHMARKS:.c=.bmk)

##
## Unibench archive generation
##

.PHONY: ub-archives
ub-archives: $(BENCHMARKS:.c=.tar)

SUFFIXES += .tar
BENCHLIB = $(abs_top_srcdir)/benchmarks/lib/benchmark.c \
           $(abs_top_srcdir)/benchmarks/lib/benchmark.h

.c.tar: $(EXTRA_DIST) $(BUILT_SOURCES) $(BENCHLIB)
	$(AM_V_at)rm -rf $@ $@.tmp
	$(AM_V_GEN)b="$<" && \
	  bn=$$(basename $$b .c) && \
	  rm -rf "$$bn" && \
	  $(MKDIR_P) "$$bn" && \
	  for f in $(EXTRA_DIST) $(BUILT_SOURCES) $(BENCHLIB); do \
	    dn=$$(dirname $$(echo $$f|$(SED) -e 's|^'$(abs_top_srcdir).*/'||g')) && \
	    $(MKDIR_P) $$bn/$$dn && \
	    cp `test -r $$f || echo $(srcdir)/`$$f $$bn/$$dn/; \
	  done && \
	  echo "a.out: $$bn.c benchmark.c; "'$$'"(COMPILER) "'$$'"(FLAGS) -I. -o "'$$'"@ "'$$'"^" \
	     >$$bn/Makefile && \
	  tardir=$$bn && $(am__tar) >$@.tmp && \
	  rm -rf $$bn
	$(AM_V_at)chmod -w $@.tmp
	$(AM_V_at)mv -f $@.tmp $@

##
## Extra
##
if ENABLE_DEMOS
noinst_DATA = $(BENCHMARKS:.c=.x)
CLEANFILES += $(BENCHMARKS:.c=.x) $(BENCHMARKS:.c=.bin.*)
endif


