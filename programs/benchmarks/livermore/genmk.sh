#! /bin/sh
set -e
exec >"ll.mk"

plist=
for k in k[1-9].sl k[1-9][0-9].sl; do
    i=$(echo $k|cut -c2-)
    for model in simple bench test; do
	plist="$plist $model$i"
    done
done
echo "DEMOS += $plist"
echo "MAINTAINERCLEANFILES += $plist"
echo "EXTRA_DIST +=" *.psize

