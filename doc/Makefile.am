## Makefile.am: this file is part of the SL toolchain.
## 
## Copyright (C) 2009 The SL project
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## The complete GNU General Public Licence Notice can be found as the
## `COPYING' file in the root directory.
##

# 
dist_man1_MANS = cce.1 slc.1 slr.1 slt.1
dist_pdf_DATA = acronyms.pdf README.pdf NEWS.pdf HACKING.pdf requirements.pdf
dist_doc_DATA = acronyms.txt README.txt NEWS.txt HACKING.txt requirements.txt \
                sl_deps.png

SUFFIXES = .txt .tex .pdf
TEXI2PDF_FLAGS = --tidy --batch --build-dir=tmp.t2d

.tex.pdf:
	cd $(srcdir); imgs=`echo *.png`; cd $(abs_builddir); \
        for i in $$imgs; do \
	  if ! test -r ./$$i; then \
            $(LN_S) $(srcdir)/$$i ./$$i; \
          fi; \
        done
	$(TEXI2PDF) $(TEXI2PDF_FLAGS) -o $@ $<

.txt.tex:
	$(RST2LATEX) --halt=warning $< >$@.tmp
	mv $@.tmp $@

cce.1 slc.1 slr.1 slt.1:
	cd $(top_builddir)/bin && $(MAKE) $(AM_MAKEFLAGS) $(@:.1=)
	rm -f $@
	$(HELP2MAN) -N --output=$@.tmp $(top_builddir)/bin/$(@:.1=)
	mv $@.tmp $@


cce.1: $(top_srcdir)/bin/cce.in
slc.1: $(top_srcdir)/bin/slc.in
slr.1: $(top_srcdir)/bin/slr.in
slt.1: $(top_srcdir)/bin/slt.in

MAINTAINERCLEANFILES = $(dist_man1_MANS) $(dist_pdf_DATA)




