#! /usr/bin/sed -f
# m4sugar_transform.sed: this file is part of the slc project.
#
# Copyright (C) 2008,2009 The SL project.
# All rights reserved.
#
# $Id$
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
