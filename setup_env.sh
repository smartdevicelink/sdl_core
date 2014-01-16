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
if [ ${uname_result} = "i386" ] || [ ${uname_result} = "i686" ]; then
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

CMAKE_BUILD_SYSTEM="cmake"
SUBVERSION="subversion"
GDEBI="gdebi"
GNU_CPP_COMPILER="g++"
BLUEZ_PROTOCOL_STACK="libbluetooth3 libbluetooth-dev"
LOG4CXX_LIBRARY="liblog4cxx10 liblog4cxx10-dev"
CHROMIUM_BROWSER="chromium-browser"
CHROMIUM_CODEC_FFMPEG="chromium-codecs-ffmpeg-extra"
PULSEAUDIO_DEV="libpulse-dev"
UPDATE_SOURCES=false
OPENGL_DEV="libgl1-mesa-dev"
APPLINK_SUBVERSION_REPO="https://adc.luxoft.com/svn/APPLINK"
CMAKE_DEB_SRC=${APPLINK_SUBVERSION_REPO}"/dist/cmake/deb"
CMAKE_DEB_DST="/tmp/cmake"
CMAKE_DATA_DEB="cmake-data_2.8.9-0ubuntu1_all.deb"
TEMP_FOLDER="/tmp"
INSTALL_ALL=false
QT_HMI=false
QNX_TARGET=false
AVAHI_CLIENT_LIBRARY="libavahi-client-dev"
DOXYGEN="doxygen"
GRAPHVIZ="graphviz"
MSCGEN="mscgen"
BLUEZ_TOOLS="bluez-tools"
GSTREAMER="gstreamer1.0*"
USB_PERMISSIONS="SUBSYSTEM==\"usb\", GROUP=\"users\", MODE=\"0666\""
LIB_UDEV="libudev-dev"
GSTREAMER="gstreamer1.0*"
USB_PERMISSIONS="SUBSYSTEM==\"usb\", GROUP=\"users\", MODE=\"0666\""
DISTRIB_CODENAME=$(grep -oP 'CODENAME=(.+)' -m 1 /etc/lsb-release | awk -F= '{ print $NF }')

GSTREAMER_REPO_LINK="deb http://ppa.launchpad.net/gstreamer-developers/ppa/ubuntu"
GSTREAMER_SRC_REPO_LINK="deb-src http://ppa.launchpad.net/gstreamer-developers/ppa/ubuntu"

FULL_GSTREAMER_REPO_LINK="$GSTREAMER_REPO_LINK $DISTRIB_CODENAME main"
FULL_GSTREAMER_SRC_REPO_LINK="$GSTREAMER_SRC_REPO_LINK $DISTRIB_CODENAME main"

while test $# -gt 0; do
        case "$1" in
                -h|--help)
                        echo "$ setup_env.sh - Installs all packages and configures system invironment for smartdevicelink core compilation"
                        echo "                 and running." 
                        echo "                 IMPORTANT: only mandatory packages will be installed if run without -a option"
                        echo " "
                        echo "$ setup_env.sh [options]"
                        echo " "
                        echo "options:"
                        echo "-h, --help                show brief help"
                        echo "-a, --all                 all mandatory and optional packages will be installed"
                        echo "-qt                       install additional packages for Qt HMI"
                        echo "-qnx                      install additional packages for QNX"
			
                        exit 0
                        ;;
                -a)
			INSTALL_ALL=true
			shift
                        ;;
                -qt)
			QT_HMI=true
			shift
                        ;;
                -qnx)
			QNX_TARGET=true
			shift
                        ;;
        esac
done

function apt-install() {
    if [ -z "$1" ];
    then
        echo "warning: apt-install() function called without parameters"
        return 1;
    fi
    set -x #Show install command to user"
    sudo apt-get install --yes --force-yes ${APT_INSTALL_FLAGS} $*
    set +x
}

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

echo "Register gstreamer repository PUBLIC KEY in system"
#sudo apt-key adv --recv-keys  --keyserver-options http-proxy="http://ods-proxy.kiev.luxoft.com:8080/" --keyserver keyserver.ubuntu.com C0B56813051D8B58
sudo apt-key add ./gstreamer.key.pub

if $UPDATE_SOURCES; then
	echo "Apdating repository..."
	sudo apt-get update
	sudo apt-get upgrade
fi

#INSTALL_CMAKE becomes "true" if no cmake  at all or lower version "2.8.9" is present
INSTALL_CMAKE=false

if dpkg -s cmake | grep installed > /dev/null; then
	echo "Checking for installed cmake"
	CMAKE_INSTALLED_VERSION=$(dpkg -s cmake | grep "^Version:" | sed "s/Version: \(.*\)/\1/")
	CMAKE_COMPARE_RESULT=$(./compare_versions.py ${CMAKE_INSTALLED_VERSION} "2.8.9")
	case ${CMAKE_COMPARE_RESULT} in
	"equal"|"1 > 2");;
	"2 > 1") echo "Removing CMake build system"
	    sudo apt-get remove -y cmake cmake-data
	    INSTALL_CMAKE=true
	    ;;
	esac
else 
	INSTALL_CMAKE=true
fi
echo $OK

if ${INSTALL_CMAKE}; then
	echo "Installing Subversion"
	apt-install ${SUBVERSION}
	echo $OK

	echo "Checking out CMake packages, please be patient"
	svn checkout ${CMAKE_DEB_SRC} ${CMAKE_DEB_DST}
	echo $OK

	echo "Installing gdebi"
	apt-install ${GDEBI}
	echo $OK

	echo "Installing CMake build system"
	if [ ${ARCH} == "i386" ]; then
	      CMAKE_DEB="cmake_2.8.9-0ubuntu1_i386.deb"
	elif [ ${ARCH} == "x64" ]; then
	      CMAKE_DEB="cmake_2.8.9-0ubuntu1_amd64.deb"
	fi
	sudo gdebi --non-interactive ${CMAKE_DEB_DST}/${CMAKE_DATA_DEB}
	sudo gdebi --non-interactive ${CMAKE_DEB_DST}/${CMAKE_DEB}
fi

echo "Installing gstreamer..."
apt-install ${GSTREAMER}
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

echo "Installing Chromium ffmpeg codec"
apt-install ${CHROMIUM_CODEC_FFMPEG}
echo $OK

echo "Installing pulseaudio development files"
apt-install ${PULSEAUDIO_DEV}
echo $OK

echo "Installing Avahi-client-dev library"
apt-install ${AVAHI_CLIENT_LIBRARY}
echo $OK

echo "Installing Libudev-dev library"
apt-install ${LIB_UDEV}
echo $OK

echo "Installing bluez tools"
apt-install ${BLUEZ_TOOLS}
echo $OK

echo "Setting up USB permissions..."
if [ ! -f "/etc/udev/rules.d/90-usbpermission.rules" ]; then
	echo "Create permission file"
	sudo touch /etc/udev/rules.d/90-usbpermission.rules
	sudo echo -e "\n" | sudo tee /etc/udev/rules.d/90-usbpermission.rules
fi

if ! grep --quiet "$USB_PERMISSIONS" /etc/udev/rules.d/90-usbpermission.rules; then
	echo "Adding permissions..."
	sudo sed -i "\$i$USB_PERMISSIONS" /etc/udev/rules.d/90-usbpermission.rules
fi

if $QT_HMI || $INSTALL_ALL; then

	echo "Installing Subversion"
	apt-install ${SUBVERSION}
	echo $OK
	
	if [ ${ARCH} = "i386" ]; then
		QT5_RUNFILE_SRC=${APPLINK_SUBVERSION_REPO}"/dist/qt5.1/runfile/i386"
		QT5_RUNFILE="qt-linux-opensource-5.1.0-x86-offline.run"
	elif [ ${ARCH} = "x64" ]; then
		QT5_RUNFILE_SRC=${APPLINK_SUBVERSION_REPO}"/dist/qt5.1/runfile/x64"
		QT5_RUNFILE="qt-linux-opensource-5.1.0-x86_64-offline.run"
	fi
	echo "Checking whether Qt5 with QML support is installed"
	qmlscene_binary=`./FindQt.sh binary qmlscene || true`
	if [ -n "$qmlscene_binary" ]; then
	  echo "Found Qt5 in "`dirname $qmlscene_binary`
	  NEED_QT5_INSTALL=false
	else
	  echo "Qt5 installation not found, you can specify it by setting environment variable CUSTOM_QT5_DIR"
	  NEED_QT5_INSTALL=true
	fi
	echo $OK

	QT5_RUNFILE_DST=${TEMP_FOLDER}"/qt5"
	QT5_RUNFILE_BIN=${QT5_RUNFILE_DST}"/"${QT5_RUNFILE}

	if $NEED_QT5_INSTALL; then

		echo "Checking out Qt5 installation runfile, please be patient"
		svn checkout ${QT5_RUNFILE_SRC} ${QT5_RUNFILE_DST}
		echo $OK

		echo "Installing Qt5 libraries"
		chmod +x ${QT5_RUNFILE_BIN}
		sudo ${QT5_RUNFILE_BIN}
		sudo updatedb
		echo $OK
	fi
	
	echo "Installing OpenGL development files"
	apt-install ${OPENGL_DEV}
	echo $OK
fi

if $QNX_TARGET || $INSTALL_ALL; then
	echo "Checking for installed QNX SDP 6.5.0"
	QNXSDP_INSTALL_FOLDER="/opt/qnx650"
	if [[ -d "${QNXSDP_INSTALL_FOLDER}" ]]; then
		echo "QNX SDP 6.5.0 already installed"
	else
	    echo "Installing wget"
		apt-install wget

		QNXSDP_TOOL_BIN="qnxsdp-6.5.0-201007091524-linux.bin"
		QNXSDP_TOOL_REPO_LINK="http://www.qnx.com/download/download/21179/"${QNXSDP_TOOL_BIN}
		QNXSDP_TOOL_RUNFILE_DST=${TEMP_FOLDER}"/QNX"
		QNXSDP_TOOL_RUNFILE_BIN=${QNXSDP_TOOL_RUNFILE_DST}"/"${QNXSDP_TOOL_BIN}

		echo "Loading QNX SDP 6.5.0 SP1 cross platform tools for Linux"
		wget -P ${QNXSDP_TOOL_RUNFILE_DST} ${QNXSDP_TOOL_REPO_LINK}

		if [ ${ARCH} = "x64" ]; then
			QNXSDP_TOOL_REQS="ia32-libs"
			echo "Installing 32-bit libraries for 64-bit OS"
			apt-install ${QNXSDP_TOOL_REQS}
		fi

		echo "Installing QNX SDP 6.5.0 SP1 cross platform tools for Linux"
		chmod +x ${QNXSDP_TOOL_RUNFILE_BIN}
		sudo ${QNXSDP_TOOL_RUNFILE_BIN}

		echo "Installing SSH server"
		SSH_SERVER="openssh-server ssh"
		apt-install ${SSH_SERVER}
	fi
	echo $OK
fi

if $INSTALL_ALL; then
	echo " "	
	echo "Installing optional packages..."
	echo " "

	echo "Installing Doxygen"
	apt-install ${DOXYGEN}
	echo $OK

	echo "Installing Graphviz for doxygen"
	apt-install ${GRAPHVIZ}
	echo $OK

	echo "Installing Mscgen"
	apt-install ${MSCGEN}
	echo $OK
fi
echo "Environment configuration successfully done!"
