#!/usr/bin/env sh
/opt/qnx650/host/linux/x86/usr/bin/ntox86-gdb -ex "target qnx $QNX_TARGET_ADDRESS:8000" -ex "upload $1 /tmp/$1" -ex "sym $1" -ex run --batch -return-child-result
