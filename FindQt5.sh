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

set -e

echo "Detecting machine architecture"
uname_result=`uname -i`
if [ ${uname_result} = "i386" ]; then
  echo "x86 machine detected"
  ARCH="i386"
elif [ ${uname_result} = "x86_64" ]; then
  echo "x64 machine detected"
  ARCH="x64"
else
  echo "unknown architecture - exit"
  exit
fi
echo
if [ ${ARCH} = "i386" ]; then
  QT5_RUNFILE_SRC=${APPLINK_SUBVERSION_REPO}"/dist/qt5.1/runfile/i386"
  QT5_RUNFILE="qt-linux-opensource-5.1.0-x86-offline.run"
elif [ ${ARCH} = "x64" ]; then
  QT5_RUNFILE_SRC=${APPLINK_SUBVERSION_REPO}"/dist/qt5.1/runfile/x64"
  QT5_RUNFILE="qt-linux-opensource-5.1.0-x86_64-offline.run"
fi
QT5_RUNFILE_DST="/tmp"
QT5_RUNFILE_BIN=${QT5_RUNFILE_DST}"/"${QT5_RUNFILE}

QT5_REQUIRED_VERSION="5.1.0"

echo "Checking whether Qt "$QT5_REQUIRED_VERSION" is installed"
NEED_QT5_INSTALL=true
qmake_list=`find -L $CUSTOM_QT5_DIR ~ /opt /usr -name qmake -type f -executable -print 2>/dev/null || true`
for qmake_binary in $qmake_list; do
  grep_result=`$qmake_binary -version | grep "Qt version "$QT5_REQUIRED_VERSION || true`
  if [ -n "$grep_result" ]; then
    QT5_DIR=`dirname $qmake_binary`
    echo "Found Qt "$QT5_REQUIRED_VERSION" in "$QT5_DIR", checking some binaries"
    QML_BINARIES_CORRUPTED=false
    for qml_binary in qmlscene qmlplugindump; do
      if [ -x $QT5_DIR/$qml_binary ]; then
        if [ -d $QT5_DIR/$qml_binary ]; then
          echo "Strange... "$qml_binary" is a directory, skipping this installation"
          QML_BINARIES_CORRUPTED=true
          break
        fi
      else
        echo $qml_binary" is either absent or non-executable, skipping this installation"
        QML_BINARIES_CORRUPTED=true
        break
      fi
    done
    if $QML_BINARIES_CORRUPTED; then
      continue
    fi
    echo "Everything is OK"
    NEED_QT5_INSTALL=false
    break
  fi
done

if $NEED_QT5_INSTALL; then

  echo "Qt "$QT5_REQUIRED_VERSION" not found, you can specify its location by setting CUSTOM_QT5_DIR environment variable"
  echo "Checking out Qt5 installation runfile, please be patient"
  svn checkout ${QT5_RUNFILE_SRC} ${QT5_RUNFILE_DST}
  echo $OK

  echo "Installing Qt5 libraries"
  chmod +x ${QT5_RUNFILE_BIN}
  sudo ${QT5_RUNFILE_BIN}
  echo $OK

fi

echo "Setting up Qt5 cmake environment:"
for module in Core DBus Qml Quick
do
  echo "module "$module"..."
  find_command_prefix="find /usr /opt / -name Qt5"
  find_command_suffix="Config.cmake -print -quit 2>/dev/null"
  find_command=$find_command_prefix$module$find_command_suffix
  find_result=`$find_command`
  file_name_prefix="cmake/Modules/FindQt5"
  file_name_suffix=".cmake"
  file_name=$file_name_prefix$module$file_name_suffix
  echo "include("$find_result")" > $file_name
done
echo $OK

