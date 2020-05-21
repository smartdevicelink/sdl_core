#!/bin/bash
cd $(dirname $0)
DIR=$(pwd)
CORE_PID_FILE=/var/run/core.pid
PM_PID_FILE=/var/run/policy_manager.pid

function core_start() {
  echo "Starting SmartDeviceLinkCore"
  LD_LIBRARY_PATH=$DIR ${DIR}/smartDeviceLinkCore &
  CORE_PID=$!
  echo $CORE_PID > $CORE_PID_FILE
}

function core_stop() {
  echo "Stopping SmartDeviceLinkCore"
  kill $(cat $CORE_PID_FILE)
  rm $CORE_PID_FILE
}

function pm_start() {
  pip3 list | grep -F tornado > /dev/null
  if [ $? -eq 1 ]; then
    echo "Installing tornado python package"
    sudo pip3 install tornado
  fi
  echo "Starting Policy Manager"
  python3 ${DIR}/sample_policy_manager.py --pack_port 8088 --unpack_port 8089 --add_http_header --encryption &
  PM_PID=$!
  echo $PM_PID > $PM_PID_FILE
}

function pm_stop() {
  echo "Stopping Policy Manager"
  kill -INT $(cat $PM_PID_FILE)
  kill -9 $(cat $PM_PID_FILE)
  rm $PM_PID_FILE
}

if [ x$1 == xstop ]; then
  core_stop
  pm_stop
elif [ x$1 == xrestart ]; then
  core_stop
  pm_stop
  core_start
  pm_start
elif [ x$1 == xstart ]; then
  core_start
  pm_start
else
  echo "usage: core.sh [start/restart/stop]"
fi
