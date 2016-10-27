#!/usr/bin/env bash
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi
echo "Change compiler version"
add-apt-repository ppa:ubuntu-toolchain-r/test -y
apt-get update
update-alternatives --remove-all gcc
update-alternatives --remove-all g++
apt-get -y install g++-4.9 gcc-4.9
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 20
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 20
update-alternatives --config gcc
update-alternatives --config g++

