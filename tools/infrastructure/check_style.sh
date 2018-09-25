#!/usr/bin/env sh
# Copyright (c) 2016 Ford Motor Company,
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of Ford Motor Company nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

FORMATER=clang-format-3.6
INSTALL_CMD="sudo apt-get install -f $FORMATER"

if [ "$1" = "--help" ]
then
    echo ""
    echo "Script checks c++ code style in all .h .cc and .cpp files"
    echo "Uses $FORMATER as base tool. Install it with : $INSTALL_CMD"
    echo "Usage: `basename $0` [option]"
    echo "      --fix   Fix files format indirectly"
    echo "      --help  Display this information"
    exit 0
fi

command -v $FORMATER >/dev/null 2>&1 || { echo >&2 "$FORMATER is not installed. Use following: $INSTALL_CMD"; exit 1; }

FILE_NAMES=$(find src -name \*.h -print -o -name \*.cpp -print  -o -name \*.cc -print | grep -v 3rd_party)


check_style() {
	$FORMATER -style=file $1 | diff $1 -
}

fix_style() {
	$FORMATER -style=file -i $1
}

if [ "$1" = "--fix" ]
then
  for FILE_NAME in $FILE_NAMES; do fix_style $FILE_NAME; done
else
  PASSED=0
  for FILE_NAME in $FILE_NAMES; do
    check_style $FILE_NAME
    if [ $? != 0 ]
    then
      echo "in " $FILE_NAME 
      PASSED=1
    fi
  done
  if [ $PASSED = 1 ]
  then 
    exit 1
  fi
fi


