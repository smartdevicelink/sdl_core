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

SUBVERSION="subversion"
GNU_CPP_COMPILER="g++"
BLUEZ_PROTOCOL_STACK="libbluetooth3 libbluetooth-dev"
LOG4CXX_LIBRARY="liblog4cxx10 liblog4cxx10-dev"
CHROMIUM_BROWSER="chromium-browser"
PULSEAUDIO_DEV="libpulse-dev"
UPDATE_SOURCES=false
OPENGL_DEV="libgl1-mesa-dev"
CMAKE_BUILD_SYSTEM="cmake"
QT5_RUNFILE="qt-linux-opensource-5.1.0-x86-offline.run"
QT5_RUNFILE_SRC="https://adc.luxoft.com/svn/APPLINK/dist/qt5.1/runfile"
QT5_RUNFILE_DST="/tmp"
QT5_RUNFILE_BIN=${QT5_RUNFILE_DST}"/"${QT5_RUNFILE}
AVAHI_CLIENT_LIBRARY="libavahi-client-dev"
AVAHI_COMMON="libavahi-common-dev"
DOXYGEN="doxygen"
GRAPHVIZ="graphviz"
MSCGEN="mscgen"


DISTRIB_CODENAME=$(grep -oP 'CODENAME=(.+)' -m 1 /etc/lsb-release | awk -F= '{ print $NF }')

GSTREAMER_REPO_LINK="deb http://ppa.launchpad.net/gstreamer-developers/ppa/ubuntu"
GSTREAMER_SRC_REPO_LINK="deb-src http://ppa.launchpad.net/gstreamer-developers/ppa/ubuntu"

FULL_GSTREAMER_REPO_LINK="$GSTREAMER_REPO_LINK $DISTRIB_CODENAME main"
FULL_GSTREAMER_SRC_REPO_LINK="$GSTREAMER_SRC_REPO_LINK $DISTRIB_CODENAME main"

function apt-install() {
    if [ -z "$1" ]
    then
        echo "warning: apt-install() function called without parameters"
        return 1;
    fi
    set -x #Show install command to user"
    sudo apt-get install --yes --force-yes ${APT_INSTALL_FLAGS} $*
    set +x
}

echo "Installng CMake build system"
apt-install ${CMAKE_BUILD_SYSTEM}
echo $OK

echo "Installng GNU C++ compiler"
apt-install ${GNU_CPP_COMPILER}
echo $OK

echo "Installng BlueZ Bluetooth protocol stack implementation"
apt-install ${BLUEZ_PROTOCOL_STACK}
echo $OK

echo "Installing log4cxx library"
apt-install ${LOG4CXX_LIBRARY}
echo $OK

echo "Installing Chromium browser"
apt-install ${CHROMIUM_BROWSER}
echo $OK

echo "Installing pulseaudio development files"
apt-install ${PULSEAUDIO_DEV}
echo $OK

echo "Installing Avahi-common-dev library"
apt-install ${AVAHI_COMMON}
echo $OK

echo "Installing Avahi-client-dev library"
apt-install ${AVAHI_CLIENT_LIBRARY}
echo $OK

echo "Installing Doxygen"
apt-install ${DOXYGEN}
echo $OK

echo "Installing Graphviz for doxygen"
apt-install ${GRAPHVIZ}
echo $OK

echo "Installing Mscgen"
apt-install ${MSCGEN}
echo $OK

echo "Installing OpenGL development files"
apt-install ${OPENGL_DEV}
echo $OK

echo "Installing Subversion"
apt-install ${SUBVERSION}
echo $OK

echo "Checking out Qt5 installation runfile, please be patient"
svn checkout ${QT5_RUNFILE_SRC} ${QT5_RUNFILE_DST}
echo $OK

echo "Installing Qt5 libraries"
chmod +x ${QT5_RUNFILE_BIN}
sudo ${QT5_RUNFILE_BIN}
echo $OK

echo "Setting up Qt5 cmake environment:"
for module in Core DBus Qml Quick
do
  echo "module "$module"..."
  find_command_prefix="find /usr /opt / -name Qt5"
  find_command_suffix="Config.cmake -print -quit"
  find_command=$find_command_prefix$module$find_command_suffix
  find_result=`$find_command`
  file_name_prefix="cmake/Modules/FindQt5"
  file_name_suffix=".cmake"
  file_name=$file_name_prefix$module$file_name_suffix
  echo "include("$find_result")" > $file_name
done
echo $OK

echo "Setting up Qt5 cmake environment"
for module in Core DBus Qml Quick
do
  find_command_prefix="find /usr /opt / -name Qt5"
  find_command_suffix="Config.cmake -print -quit"
  find_command=$find_command_prefix$module$find_command_suffix
  find_result=`$find_command`
  file_name_prefix="cmake/Modules/FindQt5"
  file_name_suffix=".cmake"
  file_name=$file_name_prefix$module$file_name_suffix
  echo "include("$find_result")" > $file_name
done

sudo cp /etc/apt/sources.list /etc/apt/sources.list.backup

if ! grep --quiet "$FULL_GSTREAMER_REPO_LINK" /etc/apt/sources.list; then
	echo "Adding gstreamer to /etc/apt/sources.list"
	sudo sed -i "\$i$FULL_GSTREAMER_REPO_LINK" /etc/apt/sources.list
	UPDATE_SOURCES=true
fi

if ! grep --quiet "$FULL_GSTREAMER_SRC_REPO_LINK" /etc/apt/sources.list; then
	echo "Adding gstreamer sources to /etc/apt/sources.list"
	sudo sed -i "\$i$FULL_GSTREAMER_SRC_REPO_LINK" /etc/apt/sources.list
	UPDATE_SOURCES=true
fi

if $UPDATE_SOURCES; then
	echo "Updating repository..."
	sudo apt-get update
fi

echo "Installing gstreamer..."
apt-install gstreamer1.0*
 


