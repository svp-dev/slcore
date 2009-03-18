dist_man1_MANS = cce.1 slc.1 sgr.1 slt.1
dist_pdf_DATA = acronyms.pdf README.pdf NEWS.pdf HACKING.pdf lang.pdf spec.pdf
dist_doc_DATA = acronyms.txt README.txt NEWS.txt HACKING.txt lang.txt spec.txt

SUFFIXES = .txt .tex .pdf
TEXI2PDF_FLAGS = --tidy --batch --build-dir=tmp.t2d

.tex.pdf:
	$(TEXI2PDF) $(TEXI2PDF_FLAGS) -o $@ $<

.txt.tex:
	$(RST2LATEX) --halt=warning $< >$@.tmp
	mv $@.tmp $@

cce.1 slc.1 sgr.1:
	cd $(top_builddir)/bin && $(MAKE) $(AM_MAKEFLAGS) $(@:.1=)
	rm -f $@
	$(HELP2MAN) -N --output=$@.tmp $(top_builddir)/bin/$(@:.1=)
	mv $@.tmp $@

slt.1:
	rm -f $@
	$(HELP2MAN) -N --output=$@.tmp $(top_srcdir)/bin/$(@:.1=)
	mv $@.tmp $@

cce.1: $(top_srcdir)/bin/cce.in
slc.1: $(top_srcdir)/bin/slc.in
sgr.1: $(top_srcdir)/bin/sgr.in
slt.1: $(top_srcdir)/bin/slt

MAINTAINERCLEANFILES = $(dist_man1_MANS) $(dist_pdf_DATA)

if MAINTAINER_MODE

BUILT_SOURCES = $(srcdir)/io.rst $(srcdir)/iomacros.rst

spec.tex: io.rst iomacros.rst

$(srcdir)/io.rst $(srcdir)/iomacros.rst: $(srcdir)/Makefile.am
	rm -f $@
	echo "- **contents of $(notdir $(@:.rst=.slh))**::" >$@
	echo >>$@
	$(SED) -e 's/^/    /g' <$(top_srcdir)/include/svp/$(notdir $(@:.rst=.slh)) >>$@

$(srcdir)/io.rst: $(top_srcdir)/include/svp/io.slh
$(srcdir)/iomacros.rst: $(top_srcdir)/include/svp/iomacros.slh

endif




