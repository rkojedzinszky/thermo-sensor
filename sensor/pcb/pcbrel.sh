#!/bin/sh

export LANG=C
V=$(git describe --tags)
V1=$(echo "$V" | cut -d- -f 1)
V2=$(echo "$V" | cut -d- -f 2-)
[ "$V1" = "$V2" ] && V2=""
ZIPFILE="$(pwd)/sensor-$V.zip"
TMP=$(mktemp -d)
rm -f "$ZIPFILE"

sed -e "s/SENSOR_VER1/$V1/g" -e "s/SENSOR_VER2/$V2/g" sensor.pcb > "$TMP/sensor.pcb"
cd "$TMP"
pcb -x gerber --gerberfile sensor sensor.pcb
zip -q9 "$ZIPFILE" sensor*
cd /
rm -rf "$TMP"
echo "$ZIPFILE"
