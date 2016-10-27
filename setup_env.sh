if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

cp /etc/apt/sources.list /etc/apt/sources.list.backup

. /etc/lsb-release
echo $DISTRIB_RELEASE

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
)

if [ $DISTRIB_RELEASE!="14.04" ]; then
   sed -i -e '$a\deb http://ppa.launchpad.net/smspillaz/cmake-2.8.12/ubuntu wily main' /etc/apt/sources.list
fi


apt-get update
apt-get install -y ${packages[@]}

if ! dpkg -s cmake | grep "Version" | grep 2.8.12; then
	apt-get remove cmake
	echo "INSTALLING CMAKE"
	CMAKE_CORRECT_VERSION="$(apt-cache show cmake | grep Version | grep '2.8.12' | awk '{print $2}')"
	apt-get install -y --force-yes cmake=$CMAKE_CORRECT_VERSION cmake-data=$CMAKE_CORRECT_VERSION
fi


. ./tools/Utils/update_gcc.sh
. ./tools/Utils/install_distcc.sh

python ./tools/infrastructure/install_hooks.py

cp /etc/apt/sources.list.backup /etc/apt/sources.list
rm -f /etc/apt/sources.list.backup