#!/bin/bash
pip3 list | grep -F tornado > /dev/null

if [ $? -eq 1 ]
  then
    echo "Installing tornado python package"
    sudo -H pip3 install tornado
fi

echo "Starting Policy Manager"
python3 sample_policy_manager.py --pack_port 8088 --unpack_port 8089 --add_http_header --encryption &
POLICY_MANAGER=$!

trap ctrl_c INT

function ctrl_c() {
        echo "Stopping SmartDeviceLinkCore"
        kill -INT $POLICY_MANAGER
        kill -9 $POLICY_MANAGER
}

echo "Starting SmartDeviceLinkCore"
LD_LIBRARY_PATH=. ./smartDeviceLinkCore
