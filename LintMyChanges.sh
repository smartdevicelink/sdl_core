#!/usr/bin/env bash

set -e

MACHINE_TYPE=`uname -m`

if [ ${MACHINE_TYPE} == 'x86_64' ]; then
	FLINT_BINARY=flint_x64
	WORDSIZE=-d__WORDSIZE=64
else
	FLINT_BINARY=flint_x86
	WORDSIZE=-d__WORDSIZE=32
fi

(cd tools/FlexeLint && make -f co-gcc.mak > /dev/null)

find ./src/ -type d -name "*include*" | awk '{print "--i../." $0}'  > ./tools/FlexeLint/sdl-include-path.lnt

echo "--i\"/usr/include/gstreamer-1.0\"" >> ./tools/FlexeLint/gcc-include-path.lnt
echo "--i\"/usr/include/glib-2.0\"" >> ./tools/FlexeLint/gcc-include-path.lnt
echo "--i\"/usr/lib/i386-linux-gnu/glib-2.0/include\"" >> ./tools/FlexeLint/gcc-include-path.lnt
echo "--i\"/usr/lib/x86_64-linux-gnu/glib-2.0/include\"" >> ./tools/FlexeLint/gcc-include-path.lnt

(git diff --name-only HEAD ; git ls-files --other --exclude-standard) | grep '.cc\|.cpp' | grep 'src/components\|src/appMain' | awk '{print "../../" $0}' > ./tools/FlexeLint/sdl-changed-modules.lnt

cd ./tools/FlexeLint/

./${FLINT_BINARY} -w1 -zero -u --i../../$1/src/components/ -dOS_POSIX ${WORDSIZE} smartdevicelink-changes.lnt 

rm ./sdl-include-path.lnt
rm ./sdl-changed-modules.lnt

make -f co-gcc.mak clean > /dev/null
