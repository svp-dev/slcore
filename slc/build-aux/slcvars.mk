## slcvars.mk: this file is part of the SL toolchain.
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

SLC_VARS = \
	SLC_INCDIR=$(SLC_INCDIR) \
	SLC_LIBDIR=$(SLC_LIBDIR) \
	SLC_DATADIR=$(SLC_DATADIR) \
	PYTHONPATH=$(PYTHONPATH) \
	SPP=$(SPP) \
	SPR=$(SPR) \
	SCU=$(SCU) \
	SAG=$(SAG) \
	CCE=$(CCE) \
	SLR=$(SLR) \
	SLT=$(SLT) \
	CM4=$(CM4) \
	SLC=$(SLC_LOCAL)

SLC_RUN = $(SLC_VARS) $(SLC_LOCAL)
