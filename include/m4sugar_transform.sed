#! /usr/bin/sed -f

# Change m4sugar's quote system; [ and ]
# are just too common in C.
s/\[/[[/g
s/\]/]]/g

# We don't care about "simplicity" of "dnl";
# "dnl" may be a valid C identifier.
s/dnl/m4_dnl/g

