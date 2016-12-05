#!/bin/bash

show_help() {
  echo "
        Usage: ./setup_rsdl.sh [OPTION]...
          Runs installers to setup environment.
        Arguments:
          -h    displays help message and exits
          -b    Install basic packages: cmake; subversion; git; g++; sqlite.
          -B    Install BlueTooth related packages: libbluetooth3 libbluetooth-dev and bluez
          -W    Install WiFi additional packages: avahi
          -U    Install USB additional packages: libudev; sets USB permissions (recommended)
          -A    Install additional packages: chromium browser for HTML HMI; SSL for security feature.
          -L    Install lua packages: lua5.2
          -a    Install all packages
          -i    Do not specify packages; prompt on each packages need to be installed

        Example:
          ./setup_rsdl.sh -b
          ./setup_rsdl.sh -a
          ./setup_rsdl.sh -bBWUA
"
}

install_base=false;
bt_install=false;
wifi_additional=false;
usb_install=false;
additional_install=false;
interact_mode=false;
lua_install=false;

while getopts "hbBWUALai" opt; do
    case "$opt" in
    h)
        show_help
        exit 0
        ;;
    b)  install_base=true
        ;;
    B)  bt_install=true
        ;;
    W)  wifi_additional=true
        ;;
    U)  usb_install=true
        ;;
    A)  additional_install=true
        ;;
    L)  lua_install=true
        ;;
    a)  install_base=true
        bt_install=true
        wifi_additional=true
        usb_install=true
        additional_install=true
        lua_install=true
        ;;
    i)  interact_mode=true
        install_base=true
        bt_install=true
        wifi_additional=true
        usb_install=true
        additional_install=true
        lua_install=true
        ;;
    esac
done

#apt-get install wrapper function
function apt-install() {
    if [ -z "$1" ]; then
        return 1;
    fi
    if ${interact_mode}; then
      APT_ARGS=""
    else
      APT_ARGS="--yes --force-yes"
    fi
    set -x #Show install command to user"
    apt-get install ${APT_ARGS} $*
    set +x
}

if ${install_base}; then
  CMAKE_BUILD_SYSTEM="cmake"
  SUBVERSION="subversion"
  GIT="git"
  GNU_CPP_COMPILER="g++"
  LIB_SQLITE="libsqlite3-dev"
else
  CMAKE_BUILD_SYSTEM=""
  SUBVERSION=""
  GIT=""
  GNU_CPP_COMPILER=""
  LIB_SQLITE=""
fi

if ${bt_install}; then
  BLUEZ_PROTOCOL_STACK="libbluetooth3 libbluetooth-dev"
  BLUEZ_TOOLS="bluez-tools"
else
  BLUEZ_PROTOCOL_STACK=""
  BLUEZ_TOOLS=""
fi

if ${wifi_additional}; then
  AVAHI_CLIENT_LIBRARY="libavahi-client-dev"
else
  AVAHI_CLIENT_LIBRARY=""
fi

if ${usb_install}; then
  LIB_UDEV="libudev-dev"
  USB_PERMISSIONS="SUBSYSTEM==\"usb\", GROUP=\"users\", MODE=\"0666\""
else
  LIB_UDEV=""
  USB_PERMISSIONS=""
fi

if ${lua_install}; then
  LUA_PACKAGE="lua5.2"
else
  LUA_PACKAGE=""
fi

if ${additional_install}; then
  CHROMIUM_BROWSER="chromium-browser"
  LIB_SSL="libssl-dev"
else
  CHROMIUM_BROWSER=""
  LIB_SSL=""
fi

#apt-get update
apt-install ${CMAKE_BUILD_SYSTEM}
apt-install ${SUBVERSION}
apt-install ${GIT}
apt-install ${GNU_CPP_COMPILER}
apt-install ${LIB_SQLITE}
apt-install ${BLUEZ_PROTOCOL_STACK}
apt-install ${BLUEZ_TOOLS}
apt-install ${AVAHI_CLIENT_LIBRARY}
apt-install ${CHROMIUM_BROWSER}
apt-install ${LIB_SSL}
apt-install ${LUA_PACKAGE}

apt-install ${LIB_UDEV}
if ${usb_install}; then
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
fi
