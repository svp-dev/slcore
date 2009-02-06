
all: inventory.pdf

inventory.pdf: inventory.ltx
	pdflatex $<
	pdflatex $<

inventory.ltx: inventory.txt
	rst2latex.py --documentoptions=a4paper,landscape $< >$@

clean:
	rm -f inventory.pdf inventory.ltx

