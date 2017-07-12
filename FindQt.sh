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

usage() {
  echo "Usage: $0 [-v <version>] [-b] [<file name>]"
  echo
  echo "       Look for file in Qt directory."
  echo "       If file name isn't set then look for bin directory."
  echo
  echo "       -v <version>  look for Qt specific version x.x.x"
  echo "       -b look for binary file"
  echo
}

version2int() {
  IFS="."
  local ver=0
  for i in $1; do
    ver=$(( $ver * 32 + $i ))
  done
  echo $ver
}

version_match() {
  v1=$(version2int $1)
  v2=$(version2int $2)
  # It's bash way to say "if ((version1 < version2) and (major1 == major2))
  if [[ ( $v1 -le $v2 ) && ( $(( ($v1 / 1024) - ($v2 / 1024) )) == 0 ) ]]; then
    return 0;
  else
    return 1;
  fi
}

qmake_data() {
  $1 --version 2>/dev/null | grep "Using Qt version" | sed "s/.*Qt version \\([0-9\\.]*\\) in \\(.*\\)$/\\$2/"
}

find_binary() {
  local qt_binary="$1/$2" # check specified binary
  if [[ -x $qt_binary && ! -d $qt_binary ]]; then # to be executable and not to be directory
    echo -n $qt_binary # output without newline
    return 0
  fi
  return 1
}

find_file() {
  local qt_file=$(find $1 -name "$2" -type f -print0 -quit 2>/dev/null) # check specified binary
  if [[ -n $qt_file && ! -d $qt_file ]]; then # if found
    echo -n $qt_file #output without newline
    return 0
  fi
  return 1
}

type=file
version="0.0.0"
while getopts :v:b option; do
  case "$option" in
    v) version=$OPTARG ;;
    b) type=binary ;;
    *) usage; exit 1; ;;
  esac
done

shift $(( OPTIND - 1 ))
if [[ -z $1 ]]; then
  type=bindir
else
  file_name=$1
fi

## First attempt - using locate
if command -v locate > /dev/null; then
  for searchloc in $CUSTOM_QT_DIR /opt /usr/local /usr; do
    qmake_list=$(locate $searchloc/*/bin/qmake)
    for qmake in $qmake_list; do
      if [[ ! -x $qmake || -d $qmake ]]; then
        continue
      fi
      # called with "qmake 1" return qmake version
      qt_version=$(qmake_data $qmake 1)
      if ! version_match $version $qt_version; then
        continue
      fi

      case $type in
        binary)
          qt_dir=$(dirname $qmake 2> /dev/null)
          if find_binary $qt_dir $file_name; then
            exit 0
          fi
          ;;
        file)
          # called with "qmake 2" return Qt installation dir
          qt_installdir=$(qmake_data $qmake 2)
          if find_file $qt_installdir $file_name; then
            exit 0
          fi
          ;;
        bindir)
          echo -n $(dirname $qmake 2>/dev/null)
          exit 0
          ;;
      esac
    done
  done
fi

## Second attempt - using find
export -f find_file
export -f qmake_data
export -f version_match
export -f version2int

qmake=$(find -L $CUSTOM_QT_DIR /opt /usr/local /usr -name '.*' -prune \
        -o -name qmake -type f \
        -executable \
        -exec /bin/bash -c "version_qt=\$(qmake_data {} 1);version_match $version \$version_qt" {} \; -print -quit > /dev/null)
if ! [ $? ]; then
   exit 1;
fi

case $type in
  binary)
    qt_dir=$(dirname $qmake 2>/dev/null)
    if find_binary $qt_dir $file_name; then
      exit 0
    fi
    ;;
  file)
    # called with "qmake 2" return Qt installation dir
    qt_installdir=$(qmake_data $qmake 2)
    if find_file $qt_installdir $file_name; then
      exit 0
    fi
    ;;
  bindir)
    echo -n $(dirname $qmake 2>/dev/null)
    exit 0
    ;;
esac

exit 1
