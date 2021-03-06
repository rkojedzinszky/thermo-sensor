#!/bin/sh

set -e

src="$1"
test -f "$src"

rm -f *.cpp *.hpp

cat > CMakeLists.txt <<EOF
# autogenerated
add_library(interrupt STATIC
EOF

for i in $(sed -n -r -e '/define.*_vect_num/{s/.*define //; s/_vect_num.*//; p}' "$src"); do
	sed -e "s/@NAME@/$i/g" int.hpp.template > $i.hpp ; sed -e "s/@NAME@/$i/g" int.cpp.template > $i.cpp
	echo " $i.cpp" >> CMakeLists.txt
done

echo ")" >> CMakeLists.txt
