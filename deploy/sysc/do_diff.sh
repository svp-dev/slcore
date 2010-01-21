#! /bin/sh
diff -ur -x Makefile.in \
	-x configure \
	-x INSTALL \
	-x aclocal.m4 \
	-x config.sub \
	-x config.guess \
	-x depcomp \
	-x install-sh \
	-x missing \
	-x mkinstalldirs \
        -x \*autom4te\* \
        -x \*~ \
	systemc-2.1.v1.orig systemc-2.1.v1-work >../patches/sysc.patch
