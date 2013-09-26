#!/usr/bin/env bash

set -e

CMAKE_BUILD_SYSTEM="cmake"
GNU_CPP_COMPILER="g++"
BLUEZ_PROTOCOL_STACK="libbluetooth3 libbluetooth-dev"
LOG4CXX_LIBRARY="liblog4cxx10 liblog4cxx10-dev"
CHROMIUM_BROWSER="chromium-browser"
PULSEAUDIO_DEV="libpulse-dev"
UPDATE_SOURCES=false
AVAHI_CLIENT_LIBRARY="libavahi-client3"
AVAHI_COMMON="libavahi-common3"
LIB_UDEV="libudev-dev"
ICECAST="icecast2"
USB_PERMISSIONS="SUBSYSTEM==\"usb\", GROUP=\"users\", MODE=\"0666\""

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

echo "Installing pulseaudio development files"
apt-install ${PULSEAUDIO_DEV}
echo $OK

echo "Installing Avahi-client library"
apt-install ${AVAHI_CLIENT_LIBRARY}
echo $OK

echo "Installing Avahi-common library"
apt-install ${AVAHI_COMMON}
echo $OK

echo "Installing Libudev-dev library"
apt-install ${LIB_UDEV}
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
apt-install gstreamer1.0*
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

echo "Installing icecast..."
apt-install ${ICECAST}
echo $OK

echo "Configuring icecast..."
sudo sed -i 's/ENABLE=false/ENABLE=true/g' /etc/default/icecast2
echo $OK

echo "Starting icecast..."
sudo /etc/init.d/icecast2 start
echo $OK

