dist_man1_MANS = cce.1 slc.1 sgr.1
dist_pdf_DATA = acronyms.pdf README.pdf
dist_doc_DATA = acronyms.txt README.txt

SUFFIXES = .txt .tex .pdf
TEXI2PDF_FLAGS = --tidy --batch --build-dir=tmp.t2d

.tex.pdf:
	$(TEXI2PDF) $(TEXI2PDF_FLAGS) -o $@ $<

.txt.tex:
	$(RST2LATEX) --halt=warning $< >$@.tmp
	mv $@.tmp $@

cce.1 slc.1 sgr.1: $(top_srcdir)/configure.ac
	cd $(top_builddir)/bin && $(MAKE) $(AM_MAKEFLAGS) $(@:.1=)
	$(HELP2MAN) -N --output=$@.tmp $(top_builddir)/bin/$(@:.1=)
	mv $@.tmp $@

cce.1: $(top_srcdir)/bin/cce.in
slc.1: $(top_srcdir)/bin/slc.in
sgr.1: $(top_srcdir)/bin/sgr.in



