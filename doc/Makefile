COMMANDS = cce sgr slc

all: acronyms.pdf tests.pdf $(COMMANDS:%=%.html) $(COMMANDS:%=%.ps)

clean:
	rm -f *.pdf *.ltx *.aux *.out *.aux *.html *.ps

.SUFFIXES: .pdf .ltx .1

%.pdf: %.ltx
	pdflatex $<
	pdflatex $<
	rm -f $(@:%.pdf=%.log) $(@:%.pdf=%.out) $(@:%.pdf=%.aux)

%.ltx: %.txt
	rst2latex.py $< >$@

%.ps: %.1
	groff -T ps -mandoc $< >$@

%.html: %.1
	groff -T html -mandoc $< >$@
