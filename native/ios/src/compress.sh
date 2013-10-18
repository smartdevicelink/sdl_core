#!/bin/bash
# Author: Eugene Nikolsky

function die () {
	echo $@
	exit 1
}

initdir=$(pwd)
cd ../../../web/

[[ "$CONFIGURATION" == "Debug" ]] && DEBUG_BUILD="1"

DSTDIR="${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}"
[[ "$DSTDIR" == "" ]] && die "Can't determine the destination directory for web files. Please check that the script is run within Xcode build process."

if [[ "$DEBUG_BUILD" ]]; then
    echo "Copying web resources to $DSTDIR…"
    cp -a src/{app,css,ffw,locale,index.html} "$DSTDIR"
else
    echo "Checking release flags…"
    egrep -q 'MCS_ENABLED:[[:blank:]]+false' src/app/Flags.js || die "Warning! Please check: MCS_ENABLED is not false!"

    echo "Checking the tools…"
    BIN_PATH=$( cd ../bin; pwd )
    [ -r "$BIN_PATH/js_compiler.jar" ] || die "Warning! Closure Compiler is not available. Please install it here: $BIN_PATH/js_compiler.jar"
    [ -r "$BIN_PATH/yuicompressor.jar" ] || die "Warning! YUI Compressor is not available. Please install it here: $BIN_PATH/yuicompressor.jar"

    # this should minify & compress all js & css files into src.min/
    # JS compression is done with Closure Compiler
    # http://code.google.com/p/closure-compiler/
    # CSS compression is done with YUI Compressor
    # http://developer.yahoo.com/yui/compressor/
    #
    # get number of cores
    cores=$(/usr/sbin/system_profiler -detailLevel full SPHardwareDataType | awk '/Total Number of Cores/ {print $5};')
    # … or 5
    cores=${cores:-5}
    echo "Compressing all js and css files…"
    # replace WARNING with note so that Xcode won't consider the warnings errors
    make -j${cores} 2>&1 | sed 's/WARNING/note:/'
    exitcode=${PIPESTATUS[0]}
    (( $exitcode == 0 )) || die "make returned exit code $exitcode"

    echo "Copying minified web resources to $DSTDIR…"
    cp -a src.min/{app,css,ffw,locale} "$DSTDIR"

    # check file size
    minfilesize=10000
    cssfilesize=$(wc -c "$DSTDIR/css/all.min.css" | awk '{print $1}')
    (( $cssfilesize >= $minfilesize )) || die "Warning! "$DSTDIR/css/all.min.css" has size of ${cssfilesize} bytes."

    echo "Generating index.html…"
    awk -f "$initdir/index.awk" src/index.html > "$DSTDIR/index.html"

    # check file size
    minfilesize=1500
    indexfilesize=$(wc -c "$DSTDIR/index.html" | awk '{print $1}')
    (( $indexfilesize >= $minfilesize )) || die "Warning! "$DSTDIR/index.html" has size of ${indexfilesize} bytes."
fi
