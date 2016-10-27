if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

if [ "$1" == "-h" ]; then
    echo "Argument -c allows to install git global name and email."
    echo "Example setup_env.sh -c username usermail"
    exit 0
fi

cp /etc/apt/sources.list /etc/apt/sources.list.backup

. /etc/lsb-release

packages=(
libssl-dev
libudev-dev
libavahi-client-dev
sqlite3
libsqlite3-dev
automake1.11
libexpat1-dev
ccache
libxml2
libxml2-dev
git
libpulse-dev
subversion
doxygen
graphviz
mscgen
libbluetooth3
libbluetooth-dev
bluez-tools
chromium-browser
libgl1-mesa-dev
libgtest-dev
binutils
)

if [ $DISTRIB_RELEASE!="14.04" ]; then
   sed -i -e '$a\deb http://ppa.launchpad.net/smspillaz/cmake-2.8.12/ubuntu wily main' /etc/apt/sources.list
fi

apt-get update
apt-get install -y ${packages[@]}

#if cmake not installed it will be installed. If any version of cmake installed except 2.8.12 it will be removed before installation.
if ! dpkg -s cmake | grep "Version" | grep 2.8.12; then
	apt-get remove cmake cmake-data
	echo "INSTALLING CMAKE"
	CMAKE_CORRECT_VERSION="$(apt-cache show cmake | grep Version | grep '2.8.12' | awk '{print $2}')"
	apt-get install -y --force-yes cmake=$CMAKE_CORRECT_VERSION cmake-data=$CMAKE_CORRECT_VERSION
fi

. ./tools/Utils/update_gcc.sh
. ./tools/Utils/install_distcc.sh

python ./tools/infrastructure/install_hooks.py

if [ "$#" -eq 3 ]; then
    if [ "$1" == "-c" ]; then
	echo "Setting up git globals"
	git config --global user.name $2
	git config --global user.email $3
    fi
fi

cp /etc/apt/sources.list.backup /etc/apt/sources.list
