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

SUBDIRS = . tools lib doc

README: $(srcdir)/doc/README.txt
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)cp $(srcdir)/doc/README.txt $@
	$(AM_V_at)chmod -w $@

NEWS: $(srcdir)/doc/NEWS.txt
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)cp $(srcdir)/doc/NEWS.txt $@
	$(AM_V_at)chmod -w $@

ACLOCAL_AMFLAGS = -I build-aux -I m4
