#!/usr/bin/env sh
# Uploads binary specified as a script argument to a QNX host and runs its remotely.
# QNX_TARGET_ADDRESS environment variable must be set to the address of target QNX host
$QNX_HOST/usr/bin/ntox86-gdb -ex "target qnx $QNX_TARGET_ADDRESS:8000" -ex "upload $1 /tmp/$1" -ex "sym $1" -ex run --batch -return-child-result
