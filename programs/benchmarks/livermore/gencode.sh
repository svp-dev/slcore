#! /bin/sh
echo >"ll.mk"
for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14; do
    cat >"bench$i.sl" <<EOF
// SLT_RUN: -f TEST.in
m4_define([[KERNEL]], $i)
m4_include(bench.sl)
EOF
    (
	echo 1;  # number of iterations
	if test -r "bench$i.bin.mtalpha.psize"; then 
	    cat "bench$i.bin.mtalpha.psize"
	else 
	    echo 1000
	fi
	) >"bench$i.sl.in"
    echo "EXTRA_DIST += bench$i.sl.in" >>"ll.mk"
    cat >"test$i.sl" <<EOF
// XIGNORE: *:D
m4_define([[KERNEL]], $i)
m4_include(test.sl)
EOF
    echo "DEMOS += bench$i.sl test$i.sl" >>"ll.mk"
    echo "MAINTAINERCLEANFILES += bench$i.sl test$i.sl bench$i.sl.in" >>"ll.mk"
done
for f in *.psize; do
    echo "EXTRA_DIST += $f" >>"ll.mk"
done

