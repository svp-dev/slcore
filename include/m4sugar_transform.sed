#! /usr/bin/sed -f

# Change m4sugar's quote system; [ and ]
# are just too common in C.
s/\[/[[/g
s/\]/]]/g

# We don't care about "simplicity" of "dnl";
# "dnl" may be a valid C identifier.
s/dnl/m4_dnl/g
s/^m4_rename_m4(\[\[ifdef\]\])/m4_rename_m4([[ifdef]])m4_rename_m4([[dnl]])/g


# Remove dependency on version.m4
s/^.*version\.m4.*$//g
