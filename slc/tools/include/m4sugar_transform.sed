#! /usr/bin/sed -f
# m4sugar_transform.sed: this file is part of the SL toolchain.
#
# Copyright (C) 2008,2009 The SL project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `COPYING' file in the root directory.
#

# Change m4sugar's quote system; [ and ]
# are just too common in C.
s/\[/[[/g
s/\]/]]/g

# We don't care about "simplicity" of "dnl";
# "dnl" may be a valid C identifier.
s/dnl/m4_dnl/g
s/^m4_rename_m4(\[\[ifdef\]\])/m4_rename_m4([[ifdef]])m4_rename_m4([[dnl]])/g


# Rewrite includes
s,m4sugar/version\.m4,slc_m4sversion.m4,g
s,m4sugar/foreach\.m4,slc_m4sforeach.m4,g
