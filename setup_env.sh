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

#Exit immediately if a command exits with a non-zero status.
set -e

INSTALL_ALL=false
INSTALL_QT_HMI=false
INSTALL_QNX_TOOLS=false

while test $# -gt 0; do
        case "$1" in
                -a|--all)
                    INSTALL_ALL=true
                    ;;
                -qt)
                    INSTALL_QT_HMI=true
                    ;;
                -qnx)
                    INSTALL_QNX_TOOLS=true
                    ;;
                -h|--help|*)
                    echo "$ setup_env.sh - Installs all packages and configures system invironment for smartdevicelink "
                    echo "                 core compilation and running."
                    echo "                 IMPORTANT: only mandatory packages will be installed if run without -a option"
                    echo " "
                    echo "Usage: setup_env.sh [option] "
                    echo "Options:"
                    echo "-h, --help            show brief help"
                    echo "-a, --all             all mandatory and optional packages will be installed"
                    echo "-qt                   install additional packages Qt HMI Qt5"
                    echo "-qnx                  install additional packages for QNX"
                    echo "-qnx -qt              install additional packages Qt HMI Qt4 and for QNX"

                    exit 0
                    ;;
        esac
    shift
done

#Check sudo
if [ $EUID != 0 ]; then
    echo "This script should be run using sudo or as the root user"
    exit 1
fi

CMAKE_BUILD_SYSTEM="cmake"
SUBVERSION="subversion"
GIT="git"
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
APPLINK_FTP_SERVER="ftp://ford-applink.luxoft.com"
CMAKE_DEB_SRC=${APPLINK_FTP_SERVER}"/Distrs/CMake/deb"
CMAKE_DEB_DST="/tmp/cmake"
TEMP_FOLDER="/tmp/APPLINK"
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
LIBXML2="libxml2-dev"

GSTREAMER_REPO_LINK="deb http://ppa.launchpad.net/gstreamer-developers/ppa/ubuntu"
GSTREAMER_SRC_REPO_LINK="deb-src http://ppa.launchpad.net/gstreamer-developers/ppa/ubuntu"

FULL_GSTREAMER_REPO_LINK="$GSTREAMER_REPO_LINK $DISTRIB_CODENAME main"
FULL_GSTREAMER_SRC_REPO_LINK="$GSTREAMER_SRC_REPO_LINK $DISTRIB_CODENAME main"

echo "Detecting machine architecture"
uname_result=`uname -i`
if [ ${uname_result} == "i386" ] || [ ${uname_result} == "i686" ]; then
    echo "x86 machine detected"
    ARCH="i386"
elif [ ${uname_result} == "x86_64" ]; then
    echo "x64 machine detected"
    ARCH="x64"
else
    echo "unknown architecture - exit"
    exit
fi
echo " "

#Indication functions for long-time operations
progress_indicator_id=0
function progress_indicator() {
    sleep_time=0.2
    echo -ne "|"
    while true; do
        echo -ne "\b|"
        sleep $sleep_time
        echo -ne "\b/"
        sleep $sleep_time
        echo -ne "\b-"
        sleep $sleep_time
        echo -ne "\b\\"
        sleep $sleep_time
    done
}
function stop_progress_indicator {
    exec 2>/dev/null
    kill $1
    echo -e "\b+"
}
function start_progress_indicator {
    echo -en "$1"
    progress_indicator &
    progress_indicator_id=$!
    trap "stop_progress_indicator $progress_indicator_id; exit" INT TERM EXIT
}
#apt-get install wrapper function
function apt-install() {
    if [ -z "$1" ]; then
        echo "warning: apt-install() function called without parameters"
        return 1;
    fi
    set -x #Show install command to user"
    apt-get install --yes --force-yes ${APT_INSTALL_FLAGS} $*
    set +x
}

echo "Installing wget"
apt-install wget
echo $OK

#Load by wget from FTP function
function load-from-ftp() {
    if [ -z "$1" ]; then
        echo "warning: load-from-ftp() function called without DOWNLOAD_LINK parameter ($1)"
        return 1;
    fi
    DOWNLOAD_LINK="$1"

    if [ -z "$2" ]; then
        echo "warning: load-from-ftp() function called without DOWNLOAD_DST parameter ($2)"
        return 2;
    fi
    DOWNLOAD_DST="$2"

    FTP_USER="sdl_user"
    FTP_PASS="sdl_user"
    wget ${DOWNLOAD_LINK} -P ${DOWNLOAD_DST} --ftp-user=${FTP_USER} --ftp-password=${FTP_PASS} --no-proxy -c
}

cp /etc/apt/sources.list /etc/apt/sources.list.backup

if ! grep --quiet "$FULL_GSTREAMER_REPO_LINK" /etc/apt/sources.list; then
    echo "Adding gstreamer to /etc/apt/sources.list"
    sed -i "\$i$FULL_GSTREAMER_REPO_LINK" /etc/apt/sources.list
    UPDATE_SOURCES=true
fi

if ! grep --quiet "$FULL_GSTREAMER_SRC_REPO_LINK" /etc/apt/sources.list; then
    echo "Adding gstreamer sources to /etc/apt/sources.list"
    sed -i "\$i$FULL_GSTREAMER_SRC_REPO_LINK" /etc/apt/sources.list
    UPDATE_SOURCES=true
fi
#hide output

echo "Register gstreamer repository PUBLIC KEY in system"
#apt-key adv --recv-keys  --keyserver-options http-proxy="http://ods-proxy.kiev.luxoft.com:8080/" --keyserver keyserver.ubuntu.com C0B56813051D8B58
apt-key add ./gstreamer.key.pub

if $UPDATE_SOURCES; then
    echo "Updating repository..."
    apt-get update
    echo "Upgrade repository..."
    apt-get upgrade --yes --force-yes
    echo $OK
fi

#Check Ubuntu version
UBUNTU_VERSION=$(lsb_release -r | sed 's/[^0-9\.]//g')
UBUNTU_VERSION_COMPARE_RESULT=$(./compare_versions.py "13.00" ${UBUNTU_VERSION} )
UBUNTU_VERSION_13_HIGHER=false
if [[ ${UBUNTU_VERSION_COMPARE_RESULT} == "2 > 1" ]]; then
    UBUNTU_VERSION_13_HIGHER=true
fi

#INSTALL_CMAKE_2_8_11_2 becomes "true" if no cmake  at all or lower version "2.8.11.2" is present
INSTALL_CMAKE_2_8_11_2=false

if dpkg -s cmake | grep installed > /dev/null; then
    echo "Checking for installed cmake"
    CMAKE_INSTALLED_VERSION=$(dpkg -s cmake | grep "^Version:" | sed "s/Version: \(.*\)/\1/")
    CMAKE_COMPARE_RESULT=$(./compare_versions.py ${CMAKE_INSTALLED_VERSION} "2.8.11.2")
    case ${CMAKE_COMPARE_RESULT} in
    "equal"|"1 > 2");;
    "2 > 1") echo "Removing CMake build system"
        apt-get remove -y cmake cmake-data
        INSTALL_CMAKE_2_8_11_2=true
        ;;
    esac
else 
    #For Ubuntu 13.0 and higher install cmake from repository
    if ${UBUNTU_VERSION_13_HIGHER} ; then
        apt-install ${CMAKE_BUILD_SYSTEM}
    else
        INSTALL_CMAKE_2_8_11_2=true
    fi
fi
echo $OK

if ${INSTALL_CMAKE_2_8_11_2}; then
    CMAKE_DATA_DEB="cmake-data_2.8.11.2-1_all.deb"
    if [ ${ARCH} == "i386" ]; then
          CMAKE_DEB="cmake_2.8.11.2-1_i386.deb"
    elif [ ${ARCH} == "x64" ]; then
          CMAKE_DEB="cmake_2.8.11.2-1_amd64.deb"
    fi

    echo "Loading CMake packages"
    load-from-ftp ${CMAKE_DEB_SRC}/${CMAKE_DATA_DEB} ${CMAKE_DEB_DST}
    load-from-ftp ${CMAKE_DEB_SRC}/${CMAKE_DEB}      ${CMAKE_DEB_DST}
    echo $OK

    echo "Installing gdebi"
    apt-install ${GDEBI}
    echo $OK

    echo "Installing CMake build system"
    gdebi --non-interactive ${CMAKE_DEB_DST}/${CMAKE_DATA_DEB}
    gdebi --non-interactive ${CMAKE_DEB_DST}/${CMAKE_DEB}
fi

echo "Installing gstreamer..."
apt-install ${GSTREAMER}
echo $OK

echo "Installing libxml2..."
apt-install ${LIBXML2}
echo $OK

echo "Installng GNU C++ compiler"
apt-install ${GNU_CPP_COMPILER}
echo $OK

echo "Installng BlueZ Bluetooth protocol stack implementation"
apt-install ${BLUEZ_PROTOCOL_STACK}
echo $OK

echo "Installing bluez tools"
apt-install ${BLUEZ_TOOLS}
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

echo "Setting up USB permissions..."
if [ ! -f "/etc/udev/rules.d/90-usbpermission.rules" ]; then
    echo "Create permission file"
    touch /etc/udev/rules.d/90-usbpermission.rules
    echo -e "\n" | tee /etc/udev/rules.d/90-usbpermission.rules
fi

if ! grep --quiet "$USB_PERMISSIONS" /etc/udev/rules.d/90-usbpermission.rules; then
    echo "Adding permissions..."
    sed -i "\$i$USB_PERMISSIONS" /etc/udev/rules.d/90-usbpermission.rules
fi

if $INSTALL_QNX_TOOLS || $INSTALL_ALL; then
    echo "Checking for installed QNX SDP 6.5.0"
    QNXSDP_INSTALL_FOLDER="/opt/qnx650"
    if [[ -d "${QNXSDP_INSTALL_FOLDER}" ]]; then
       echo "QNX SDP 6.5.0 already installed"
    else
        if [ ${ARCH} == "x64" ]; then
            echo "Installing 32-bit libraries for 64-bit OS"
            #For Ubuntu 13.0 and higher install ia32-libs from archive
            if ${UBUNTU_VERSION_13_HIGHER} ; then
                QNXSDP_TOOL_REQS="lib32z1 lib32ncurses5 lib32bz2-1.0"
            else
                QNXSDP_TOOL_REQS="ia32-libs"
            fi
            apt-install ${QNXSDP_TOOL_REQS}
        fi

        QNXSDP_TOOL_BIN="qnxsdp-6.5.0-201007091524-linux.bin"
        QNXSDP_TOOL_REPO_LINK="${APPLINK_FTP_SERVER}/Distrs/QNX/${QNXSDP_TOOL_BIN}"
        QNXSDP_TOOL_RUNFILE_DST="${TEMP_FOLDER}/QNX"
        QNXSDP_TOOL_RUNFILE_BIN="${QNXSDP_TOOL_RUNFILE_DST}/${QNXSDP_TOOL_BIN}"

        echo "Loading QNX SDP 6.5.0 SP1 cross platform tools for Linux"
        load-from-ftp ${QNXSDP_TOOL_REPO_LINK}  ${QNXSDP_TOOL_RUNFILE_DST}

        echo "Installing QNX SDP 6.5.0 SP1 cross platform tools for Linux"
        chmod +x ${QNXSDP_TOOL_RUNFILE_BIN}
        ${QNXSDP_TOOL_RUNFILE_BIN} -silent

        echo "Installing SSH server"
        SSH_SERVER="openssh-server ssh"
        apt-install ${SSH_SERVER}
    fi

    #Update system variable QNX_TARGET
    source /etc/profile
    echo $OK
fi

QT5_HMI=false
QT4_HMI=false

if $INSTALL_ALL; then
   QT5_HMI=true
   QT4_HMI=true
elif $INSTALL_QT_HMI; then
    if $INSTALL_QNX_TOOLS; then
        QT4_HMI=true
    else
        QT5_HMI=true
    fi
fi

if $QT5_HMI; then
    echo ""
    echo "Checking for installed Qt5 with QML support"
    NEED_QT5_INSTALL=false
    qmlscene_binary=`./FindQt.sh -v 5.1.0 -b qmlscene || true`
    if [ -n "$qmlscene_binary" ]; then
        echo "Found Qt5 in "`dirname $qmlscene_binary`
    else
        NEED_QT5_INSTALL=true
    fi
    echo $OK

    if $NEED_QT5_INSTALL; then
        if [ ${ARCH} == "i386" ]; then
            QT5_RUNFILE="qt-linux-opensource-5.1.0-x86-offline.run"
            QT5_RUNFILE_SRC="${APPLINK_FTP_SERVER}/Distrs/Qt5.1/runfile/i386"
        elif [ ${ARCH} == "x64" ]; then
            QT5_RUNFILE="qt-linux-opensource-5.1.0-x86_64-offline.run"
            QT5_RUNFILE_SRC="${APPLINK_FTP_SERVER}/Distrs/Qt5.1/runfile/x64"
        fi
        QT5_RUNFILE_DST=${TEMP_FOLDER}"/qt5"
        QT5_RUNFILE_BIN=${QT5_RUNFILE_DST}"/"${QT5_RUNFILE}

        echo "Download Qt5 installation runfile, please be patient"
        load-from-ftp ${QT5_RUNFILE_SRC}/${QT5_RUNFILE}  ${QT5_RUNFILE_DST}
        echo $OK

        echo "Installing Qt5 libraries"
        chmod +x ${QT5_RUNFILE_BIN}
        ${QT5_RUNFILE_BIN}
        #Update location of Qt
        updatedb
        echo $OK
    fi

    echo "Installing OpenGL development files"
    apt-install ${OPENGL_DEV}
    echo $OK
fi

if $QT4_HMI; then
    NEED_QT4_INSTALL=false
    qmlscene_binary=`./FindQt.sh -v 4.8.5 -b qmlviewer || true`
    if [ -n "$qmlscene_binary" ]; then
        echo "Found Qt4 in "`dirname $qmlscene_binary`
    else
        NEED_QT4_INSTALL=true
    fi

    if $NEED_QT4_INSTALL; then
        #Save current directory
        pushd .

        #Load system variable QNX_TARGET to root for correct cross-build
        source /etc/profile

        THIRDPARTYLIBS_DOWNLOAD_LINK="${APPLINK_FTP_SERVER}/Distrs/thirdPartyLibs"
        BUILD_THREADS_COUNT=$(($(nproc)+1))

        EXPAT_FOLDER="expat-2.1.0"
        EXPAT_ARCHIVE="${EXPAT_FOLDER}.tar.gz"
        EXPAT_DOWNLOAD_LINK="${THIRDPARTYLIBS_DOWNLOAD_LINK}/${EXPAT_ARCHIVE}"
        EXPAT_DOWNLOAD_DST="${TEMP_FOLDER}/expat"
        load-from-ftp ${EXPAT_DOWNLOAD_LINK} ${EXPAT_DOWNLOAD_DST}
        EXPAT_BUILD_LOG="${EXPAT_DOWNLOAD_DST}/${EXPAT_FOLDER}_build.log"
        echo "Installing EXPAT, please be patient."
        echo "Additional configure and build information will be saved to ${EXPAT_BUILD_LOG}"
        start_progress_indicator
        {
        cd ${EXPAT_DOWNLOAD_DST}
        tar -xzf ${EXPAT_ARCHIVE}
        cd ${EXPAT_FOLDER}
        ./configure --prefix=${QNX_TARGET}/usr --host=x86-nto CC=ntox86-gcc
        make -j${BUILD_THREADS_COUNT}
        make installlib
        #save configure and make output in log file
        } &> ${EXPAT_BUILD_LOG}
        stop_progress_indicator $progress_indicator_id

        DBUS_FOLDER="dbus-1.7.8"
        DBUS_ARCHIVE="${DBUS_FOLDER}.tar.gz"
        DBUS_DOWNLOAD_LINK="${THIRDPARTYLIBS_DOWNLOAD_LINK}/${DBUS_ARCHIVE}"
        DBUS_DOWNLOAD_DST="${TEMP_FOLDER}/dbus"
        load-from-ftp ${DBUS_DOWNLOAD_LINK}  ${DBUS_DOWNLOAD_DST}
        DBUS_BUILD_LOG="${DBUS_DOWNLOAD_DST}/${DBUS_FOLDER}_build.log"
        echo "Installing DBUS, please be patient."
        echo "Additional configure and build information will be saved to ${DBUS_BUILD_LOG}"
        start_progress_indicator
        {
        cd ${DBUS_DOWNLOAD_DST}
        tar -xzf ${DBUS_ARCHIVE}
        cd ${DBUS_FOLDER}
        ./configure --prefix=${QNX_TARGET}/usr --host=x86-nto CC=ntox86-gcc \
        LDFLAGS='-L${QNX_TARGET}/usr/lib' CFLAGS='-I${QNX_TARGET}/usr/include' --disable-tests
        make -j${BUILD_THREADS_COUNT}
        make install
        #save configure and make output in log file
        } &> ${DBUS_BUILD_LOG}
        stop_progress_indicator $progress_indicator_id

        QT4_FOLDER="qt-everywhere-opensource-src-4.8.5"
        QT4_ARCHIVE="${QT4_FOLDER}.tar.gz"
        QT4_DOWNLOAD_LINK="$APPLINK_FTP_SERVER/Distrs/Qt4.8.5/${QT4_ARCHIVE}"
        QT4_DOWNLOAD_DST="${TEMP_FOLDER}/qt4"
        load-from-ftp ${QT4_DOWNLOAD_LINK}  ${QT4_DOWNLOAD_DST}
        QT4_BUILD_LOG="${QT4_DOWNLOAD_DST}/${QT4_FOLDER}_build.log"
        echo "Installing Qt4, please be patient."
        echo "Additional configure and build information will be saved to ${QT4_BUILD_LOG}"
        start_progress_indicator
        {
        cd ${QT4_DOWNLOAD_DST}
        tar -xzf ${QT4_ARCHIVE}
        cd ${QT4_FOLDER}
        ./configure  -prefix ${QNX_TARGET}/usr/local/Qt-4.8.5 -xplatform qws/qnx-i386-g++ -embedded x86 -no-gfx-linuxfb \
            -no-mouse-linuxtp -no-kbd-tty -no-qt3support -qt-gfx-qnx -qt-mouse-qnx -qt-kbd-qnx -opensource -confirm-license \
            -no-webkit -dbus -opengl es2 -no-openvg -nomake examples -nomake demos \
            -L ${QNX_TARGET}/usr/lib/ -ldbus-1 -I ${QNX_TARGET}/usr/lib/dbus-1.0/include/ -I ${QNX_TARGET}/usr/include/dbus-1.0/ 
        make -j${BUILD_THREADS_COUNT}
        make install
        cd tools/qml
        ../../bin/qmake
        make || true
        cp ../../bin/qmlviewer ${QNX_TARGET}/usr/local/Qt-4.8.5/bin/
        #save configure and make output in log file
        } &> ${QT4_BUILD_LOG}
        stop_progress_indicator $progress_indicator_id

        #Load correct current directory
        popd

        #Update location of Qt
        updatedb
        fi

    echo $OK
fi

if $INSTALL_ALL; then
    echo " "
    echo "Installing optional packages..."
    echo " "

    echo "Installing Subversion"
    apt-install ${SUBVERSION}
    echo $OK

    echo "Installing Git"
    apt-install ${GIT}
    echo $OK

    echo "Installing Doxygen"
    apt-install ${DOXYGEN}
    echo $OK

    echo "Installing Graphviz for Doxygen"
    apt-install ${GRAPHVIZ}
    echo $OK

    echo "Installing Mscgen"
    apt-install ${MSCGEN}
    echo $OK
fi
echo "Environment configuration successfully done!"
