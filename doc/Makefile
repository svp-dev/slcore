
all: inventory.pdf tests.pdf

%.pdf: %.ltx
	pdflatex $<
	pdflatex $<

%.ltx: %.txt
	rst2latex.py --documentoptions=a4paper,landscape $< >$@

clean:
	rm -f *.pdf *.ltx

