#! /bin/sh
set -e
echo "Generating Livermore loop programs..."
exec >"ll.mk"

plist=
ilist=
for model in simple bench test; do
    for k in k[1-9].c k[1-9][0-9].c; do
	i=$(echo $k|cut -c2-)
	plist="$plist $model$i"
    done
done
for k in k[1-9].c k[1-9][0-9].c; do
    i=$(echo $k|cut -c2-)
    ilist="$ilist bench$i.in"
done
echo "check_DATA += $ilist"
echo "DEMOS += $plist"
echo "DISTCLEANFILES += $ilist $plist"
echo "EXTRA_DIST +=" *.psize k[1-9].c k[1-9][0-9].c

