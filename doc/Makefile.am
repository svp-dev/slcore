dist_man1_MANS = cce.1 slc.1 sgr.1
dist_pdf_DATA = acronyms.pdf README.pdf NEWS.pdf HACKING.pdf lang.pdf spec.pdf
dist_doc_DATA = acronyms.txt README.txt NEWS.txt HACKING.txt lang.txt spec.txt

SUFFIXES = .txt .tex .pdf
TEXI2PDF_FLAGS = --tidy --batch --build-dir=tmp.t2d

.tex.pdf:
	$(TEXI2PDF) $(TEXI2PDF_FLAGS) -o $@ $<

.txt.tex:
	$(RST2LATEX) --halt=warning $< >$@.tmp
	mv $@.tmp $@

$(dist_man1_MANS): 
	cd $(top_builddir)/bin && $(MAKE) $(AM_MAKEFLAGS) $(@:.1=)
	$(HELP2MAN) -N --output=$@.tmp $(top_builddir)/bin/$(@:.1=)
	mv $@.tmp $@

cce.1: $(top_srcdir)/bin/cce.in
slc.1: $(top_srcdir)/bin/slc.in
sgr.1: $(top_srcdir)/bin/sgr.in

MAINTAINERCLEANFILES = $(dist_man1_MANS) $(dist_pdf_DATA)

if MAINTAINER_MODE

BUILT_SOURCES = io.rst iomacros.rst

spec.tex: io.rst iomacros.rst

io.rst iomacros.rst: $(srcdir)/Makefile.am
	rm -f $@
	echo "- **contents of $(@:.rst=.slh)**::" >$@
	echo >>$@
	$(SED) -e 's/^/    /g' <$(top_srcdir)/include/svp/$(@:.rst=.slh) >>$@

io.rst: $(top_srcdir)/include/svp/io.slh
iomacros.rst: $(top_srcdir)/include/svp/iomacros.slh

endif




