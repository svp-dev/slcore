## Makefile.am: this file is part of the SL toolchain.
## 
## Copyright (C) 2009,2010,2011,2012 The SL project
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
dist_pdf_DATA = acronyms.pdf README.pdf NEWS.pdf HACKING.pdf 
dist_doc_DATA = acronyms.rst README.rst NEWS.rst HACKING.rst sl_deps.png

SUFFIXES = .rst .tex .pdf
TEXI2PDF_FLAGS = --tidy --batch --build-dir=tmp.t2d

.tex.pdf:
	cd $(srcdir); imgs=`echo *.png`; cd $(abs_builddir); \
        for i in $$imgs; do \
	  if ! test -r ./$$i; then \
            $(LN_S) $(srcdir)/$$i ./$$i; \
          fi; \
        done
	$(TEXI2PDF) $(TEXI2PDF_FLAGS) -o $@ $<

.rst.tex:
	$(RST2LATEX) --halt=warning $< >$@.tmp
	mv $@.tmp $@


MAINTAINERCLEANFILES = $(dist_pdf_DATA)
