#!/bin/bash
python sample_policy_manager.py --pack_port 8088 --unpack_port 8089 &
POLICY_MANAGER=$!

trap ctrl_c INT

function ctrl_c() {
        echo "Stopping SmartDeviceLinkCore"
        kill -INT $POLICY_MANAGER
        kill -9 $POLICY_MANAGER
}

LD_LIBRARY_PATH=. ./smartDeviceLinkCore
