#!/usr/bin/env bash

# Copyright (c) 2013, Ford Motor Company
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following
# disclaimer in the documentation and/or other materials provided with the
# distribution.
#
# Neither the name of the Ford Motor Company nor the names of its contributors
# may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

QT5_REQUIRED_VERSION="5.1.0"

version2int()
{
    IFS="."
    local ver=0
    for i in $1; do
        ver=$(( $ver * 32 + $i ))
    done
    echo $ver
}

versionmatch()
{
    if [[ $(version2int $1) -ge $(version2int $QT5_REQUIRED_VERSION) ]]; then
        return 0;
    else
        return 1;
    fi
}

# called with "qmake 1" return qmake version
# called with "qmake 2" return Qt installation dir
qmakedata()
{
    $1 --version | grep "Using Qt version" | sed "s/.*Qt version \\([0-9\\.]*\\) in \\(.*\\)$/\\$2/"
}


findfiles()
{
    local qmake_binary=$1
    if [ -x $qmake_binary -a ! -d $qmake_binary ]; then
        local VERSION=$(qmakedata $qmake_binary 1);
        local QT5_DIR=$(dirname $qmake_binary)
        local QT5_INSTALLDIR=$(qmakedata $qmake_binary 2);
        if versionmatch $VERSION; then
            case $2 in
            binary)
                qt5_binary=$QT5_DIR/$3 # check specified binary
                if [ -x $qt5_binary -a ! -d $qt5_binary ]; then # to be executable and not to be directory
                    echo -n $qt5_binary # output without newline
                    exit 0
                fi
                ;;
            file)
                qt5_file=$(find $QT5_INSTALLDIR -name "$3" -type f -print0 -quit) # check specified binary
                if [ -n "$qt5_file" -a ! -d "$qt5_file" ]; then # if found
                    echo -n $qt5_file #output without newline
                    exit 0
                fi
                ;;
            esac
        fi
    fi
    exit 1
}

if [ -z $1 ]; then # if no argument specified
    exit 0 # we are happy already
fi

if [ ! $1 == "binary" -a ! $1 == "file" -o -z $2 ]; then
    echo "Syntax error: using FindQt5.sh (binary|file) filename"
    exit 1
fi

if [[ $(command -v locate) ]]; then
    ## First attempt - using locate

    qmake_list=$(locate bin/qmake)
    for qmake in $qmake_list; do
        if findfiles $qmake $1 $2; then
            exit 0
        fi
    done
fi

## Second attempt - using find

export -f findfiles
export -f qmakedata
export -f versionmatch
export -f version2int
if find -L $CUSTOM_QT5_DIR ~ /opt /usr -name '.*' -prune \
        -o -name qmake -type f \
        -executable \
        -exec /bin/bash -c "findfiles \$0 $1 $2" {} \; -quit; then
   exit 0;
fi
exit 1
