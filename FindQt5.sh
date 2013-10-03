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

qmake_list=`find -L $CUSTOM_QT5_DIR ~ /opt /usr -name qmake -type f -executable -print 2>/dev/null || true` # first we look for qmake binary
for qmake_binary in $qmake_list; do # for all candidates
  grep_result=`$qmake_binary -version | grep "Qt version "$QT5_REQUIRED_VERSION || true` # ask version (only qmake provides this option for sure)
  if [ -n "$grep_result" ]; then # if version matches
    if [ -z $1 ]; then # if no argument specified
      exit 0 # we are happy already
    fi
    QT5_DIR=`dirname $qmake_binary`
    qt5_binary=$QT5_DIR/$1 # check specified binary
    if [ -x $qt5_binary ]; then # to be executable
      if ! [ -d $qt5_binary ]; then # and not to be directory
        echo $qt5_binary # output
        exit 0
      fi
    fi
  fi
done
exit 1 # haven't found anything
