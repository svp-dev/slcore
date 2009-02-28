#! /bin/bash
set -e
source=${1:?}

code=$( (
cat <<EOF
# Alpha integer division as thread functions.
# This code is a simplified / cleanup version
# of the result of compiling divide.c
#
EOF
grep -v "# IGNORE" < "$source")|grep -v "# MT"|grep -v '\.set.*macro'|grep -v '\.section' \
    | grep -v "# PV"|grep -v "# SP"|grep -v "gpdisp" \
    | sed -e 's/align 2/align 4/g' \
    | sed -e 's/prologue 1/prologue 0/g' \
    | tr '\n' '@' | sed  -e '
# document functions
s/.globl __divmodqu/.globl __divmodqu@##@## 64-bit unsigned division@##/g;
s/.globl __divmodlu/.globl __divmodlu@##@## 32-bit unsigned division@##/g;
s/.globl __divmodqs/.globl __divmodqs@##@## 64-bit signed division@##/g;
s/.globl __divmodls/.globl __divmodls@##@## 32-bit signed division@##/g;
' )

for t in qu lu qs ls; do
    echo "working on __divmod$t:" >&2
    echo "- extracting..." >&2
    fcode=$(echo "$code"|sed -e "s/^.*__divmod$t:"'\(.*\)'"__divmod$t.*$"'/\1/g')
    echo "- transforming to MT..." >&2
    fcode=$(echo "$fcode"| sed -E '
# add end bit after halt instruction
#s/@[^a-z@]*.int 0x74000000[^@]*@[^@]*/@	.int 0x74000000@	end/g;
# replace "nop; end" by "end"
s/@[^a-z@]*nop@[^a-z@]*end[^@]*/@	end/g;
# remove spurious ret instruction
s/@[^a-z@]*ret[^@]*/@/g;
# add swch after branches
s/@[^a-z@]*(br|bne|bge|beq|ble|blt|bgt) ([^@]*)/@	\1 \2@	swch/g;
#s/@[^a-z@]*(br|bne|bge|beq|ble|blt|bgt) ([^@]*)/@	\1 \2@	swch/g;
')
    echo "- adding swch after first use of dependents..." >&2
    for ((d=0;d<32;d++)); do
	fcode=$(echo "$fcode"|sed -E -e '
s/@([^@]*\$d'$d'[^0-9@][^@]*)@/@\1@	swch@/;')
    done
    echo "- trying to optimize .registers..." >&2
    echo "  - currently uses #locals = " $(echo "$fcode"|sed -e 's/.*registers [0-9]* [0-9]* \([0-9]*\).*/\1/g') >&2
    for ((r=32;r>=0;r--)); do
	search=$(echo "$fcode"|sed -e 's/^.*\$l'$r'.*$/YAY/g')
	if test "x$search" = xYAY; then
	    break
	fi
    done
    r=$(expr $r + 1)
    echo "  - replace to use #locals = $r" >&2
    fcode=$(echo "$fcode"|sed -e 's/@[^@].*\.registers[^@]*@/@	.registers 0 2 '$r' 0 0 0@/g')
    echo "- replacing code..." >&2
    code=$(echo "$code"|sed -e 's/__divmod'$t':\(.*\)__divmod'$t'/__divmod'$t':'"$fcode"'__divmod'$t'/g')
    echo "done." >&2
done

echo "$code"|tr '@' '\n'
