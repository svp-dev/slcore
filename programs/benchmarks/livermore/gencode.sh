#! /bin/sh
echo >"ll.mk"
for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14; do
    cat >"bench$i.sl" <<EOF
m4_define([[KERNEL]], $i)
m4_include(bench.sl)
EOF
    cat >"test$i.sl" <<EOF
m4_define([[KERNEL]], $i)
m4_include(test.sl)
EOF
    echo "DEMOS += bench$i.sl test$i.sl" >>"ll.mk"
done
