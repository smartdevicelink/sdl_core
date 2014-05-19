# This script star QDB server for SDL
# Need superuser to start qdb

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/qnx650/target/qnx6/x86/usr/lib:.
export LD_LIBRARY_PATH
/usr/sbin/qdb -c policy.ini
