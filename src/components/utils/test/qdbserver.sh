# This script star QDB server for SDL
# Need superuser to start qdb

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/qnx650/target/qnx6/x86/usr/lib
export LD_LIBRARY_PATH
/usr/sbin/qdb -c test-qdb.ini
