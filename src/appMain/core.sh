#!/bin/bash
cd $(dirname $0)
DIR=$(pwd)
PID_DIR=~/.sdl

if [ ! -d "$PID_DIR" ]; then
  mkdir $PID_DIR
fi

CORE_PID_FILE=${PID_DIR}/core.pid
CORE_PROCESS_NAME=SDLCore

function core_start() {
  if [ -f "$CORE_PID_FILE" ] && [ -n "$(ps -p $(cat $CORE_PID_FILE) -o pid=)" ]; then
    echo "Core is already running"
    return 1
  elif [ -n "$(pgrep $CORE_PROCESS_NAME)" ]; then
    echo "Core is already running outside of this script"
    echo "This lingering instance can be stopped using the \"kill\" command"
    return 2
  else
    echo "Starting SmartDeviceLinkCore"
    LD_LIBRARY_PATH=$DIR ${DIR}/smartDeviceLinkCore &
    CORE_PID=$!
    echo $CORE_PID > $CORE_PID_FILE
    return 0
  fi
}

function core_stop() {
  RESULT=1
  if [ -f "$CORE_PID_FILE" ] && [ -n "$(ps -p $(cat $CORE_PID_FILE) -o pid=)" ]; then
    echo "Stopping SmartDeviceLinkCore"
    CORE_PID=$(cat $CORE_PID_FILE)
    kill $CORE_PID

    # If Core doesn't close normally within 10 seconds, force-close it
    timeout 10s tail --pid=$CORE_PID -f /dev/null
    if [ -n "$(ps -p $CORE_PID -o pid=)" ]; then
      echo "Core did not shut down properly, force-killing"
      kill -9 $CORE_PID
    fi
    RESULT=0
  fi

  if [ -f "$CORE_PID_FILE" ]; then
    rm $CORE_PID_FILE
  fi 
  return $RESULT
}

if [ x$1 == xstop ]; then
  core_stop
  if [ ! "$?" -eq 0 ]; then
    echo "Core is not running (or was started outside of this script)"
  fi
elif [ x$1 == xrestart ]; then
  core_stop
  if [ "$?" -eq 0 ]; then
    core_start
  else
    echo "Core is not running (or was started outside of this script)"
  fi
elif [ x$1 == xstart ]; then
  core_start
elif [ x$1 == xkill ]; then
  core_stop
  if [ -n "$(pgrep $CORE_PROCESS_NAME)" ]; then
    echo "Killing all lingering instances of SDL Core"
    killall -9 $CORE_PROCESS_NAME
  fi
else
  echo "usage: core.sh [start/restart/stop/kill]"
fi
