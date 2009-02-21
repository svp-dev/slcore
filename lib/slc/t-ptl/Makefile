all: ptl_create.h

clean:
	rm -f ptl_create.h

ptl_create.h: template.pl create.tpl
	perl template.pl <create.tpl >$@
