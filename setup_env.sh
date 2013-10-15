#!/usr/bin/env bash

set -e

CMAKE_BUILD_SYSTEM="cmake"
GNU_CPP_COMPILER="g++"
BLUEZ_PROTOCOL_STACK="libbluetooth3 libbluetooth-dev"
LOG4CXX_LIBRARY="liblog4cxx10 liblog4cxx10-dev"
CHROMIUM_BROWSER="chromium-browser"
CHROMIUM_CODEC_FFMPEG="chromium-codecs-ffmpeg-extra"
PULSEAUDIO_DEV="libpulse-dev"
UPDATE_SOURCES=false
INSTALL_ALL=false
AVAHI_CLIENT_LIBRARY="libavahi-client-dev"
DOXYGEN="doxygen"
GRAPHVIZ="graphviz"
MSCGEN="mscgen"
BLUEZ_TOOLS="bluez-tools"
LIB_UDEV="libudev-dev"
ICECAST="icecast2"
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
                        echo "$ setup_enc.sh - Installs all packages and configures system invironment for smartdevicelink core compilation"
                        echo "                 and running." 
                        echo "                 IMPORTANT: only mandatory packages will be installed if run without -a option"
                        echo " "
                        echo "$ setup_enc.sh [options]"
                        echo " "
                        echo "options:"
                        echo "-h, --help                show brief help"
                        echo "-a, --all                 all mandatory and optional packages will be install"
			
                        exit 0
                        ;;
                -a)
			INSTALL_ALL=true
			shift
                        ;;
        esac
done

function apt-install() {
    if [ -z "$1" ]
    then
        echo "warning: apt-install() function called without parameters"
        return 1;
    fi
    set -x #Show install command to user"
    sudo apt-get install --yes ${APT_INSTALL_FLAGS} $*
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
echo "Installing bluez tools"
apt-install ${BLUEZ_TOOLS}
echo $OK

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
	echo "Apdating repository..."
	sudo apt-get update
fi

echo "Installing gstreamer..."
apt-install ${GSTREAMER}
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

	echo "Installing icecast..."
	apt-install ${ICECAST}
	echo $OK

	echo "Configuring icecast..."
	sudo sed -i 's/ENABLE=false/ENABLE=true/g' /etc/default/icecast2
	echo $OK

	echo "Starting icecast..."
	sudo /etc/init.d/icecast2 start
	echo $OK
fi

