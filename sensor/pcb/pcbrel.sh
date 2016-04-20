#!/bin/sh

export LANG=C
V=$(git describe --tags)
ZIPFILE="$(pwd)/sensor-$V.zip"
TMP=$(mktemp -d)
rm -f "$ZIPFILE"

sed -e "s/SENSOR_VERSION/$V/g" sensor.pcb > "$TMP/sensor.pcb"
cd "$TMP"
pcb -x gerber --gerberfile sensor sensor.pcb
rm -f sensor.pcb
zip -9 "$ZIPFILE" sensor*
cd /
rm -rf "$TMP"
echo "$ZIPFILE"
