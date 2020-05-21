#!/bin/bash
cd $(dirname $0)
DIR=$(pwd)
PID_FILE=$DIR/core.pid

function core_start() {
  echo "Starting SmartDeviceLinkCore"
  LD_LIBRARY_PATH=$DIR ${DIR}/smartDeviceLinkCore &
  PID=$!
  echo $PID > $PID_FILE
}

function core_stop() {
  echo "Stopping SmartDeviceLinkCore"
  kill $(cat $PID_FILE)
  rm $PID_FILE
}

if [ x$1 == xstop ]; then
  core_stop
elif [ x$1 == xrestart ]; then
  core_stop
  core_start
elif [ x$1 == xstart ]; then
  core_start
else
  echo "usage: core.sh [start/restart/stop]"
fi
