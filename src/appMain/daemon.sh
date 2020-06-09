#!/bin/bash
cd $(dirname $0)
COMMAND=$1
EXTERNAL_POLICIES=$2
DIR=$(pwd)

CORE_PID_FILE=${DIR}/core.pid
CORE_APPLICATION_NAME=smartDeviceLinkCore
PM_PID_FILE=${DIR}/policy_manager.pid
PM_APPLICATION_NAME=sample_policy_manager.py

function core_start() {
  if [ -f "$CORE_PID_FILE" ] && [ -n "$(ps -p $(cat $CORE_PID_FILE) -o pid=)" ]; then
    echo "Core is already running"
    return 1
  elif [ -n "$(pgrep -f $CORE_APPLICATION_NAME)" ]; then
    echo "Core is already running outside of this script"
    echo "All instances of Core can be stopped using the \"kill\" command"
    return 2
  else
    echo "Starting SmartDeviceLink Core"
    LD_LIBRARY_PATH=$DIR ${DIR}/${CORE_APPLICATION_NAME} &
    CORE_PID=$!
    echo $CORE_PID > $CORE_PID_FILE
    return 0
  fi
}

function core_stop() {
  RESULT=1
  if [ -f "$CORE_PID_FILE" ] && [ -n "$(ps -p $(cat $CORE_PID_FILE) -o pid=)" ]; then
    echo "Stopping SmartDeviceLink Core"
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

function pm_install_dependencies() {
  pip3 list | grep -F tornado > /dev/null
  if [ $? -eq 1 ]; then
    echo "Installing tornado python package"
    sudo -H pip3 install tornado
  fi
}

function pm_start() {
  if [ -f "$PM_PID_FILE" ] && [ -n "$(ps -p $(cat $PM_PID_FILE) -o pid=)" ]; then
    echo "Policy Server is already running"
    return 1
  elif [ -n "$(pgrep -f $PM_APPLICATION_NAME)" ]; then
    echo "Policy Server is already running outside of this script"
    echo "All instances of Core can be stopped using the \"kill\" command"
    return 2
  else
    pm_install_dependencies
    echo "Starting Policy Manager"
    python3 ${DIR}/${PM_APPLICATION_NAME} --pack_port 8088 --unpack_port 8089 --add_http_header --encryption &
    PM_PID=$!
    echo $PM_PID > $PM_PID_FILE
    return 0
  fi
}

function pm_stop() {
  RESULT=1
  if [ -f "$PM_PID_FILE" ] && [ -n "$(ps -p $(cat $PM_PID_FILE) -o pid=)" ]; then
    echo "Stopping Policy Manager"
    kill -INT $(cat $PM_PID_FILE)
    kill -9 $(cat $PM_PID_FILE)
    RESULT=0
  fi
  
  # Clear PID regardless of whether the process was running or not
  if [ -f "$PM_PID_FILE" ]; then
    rm $PM_PID_FILE
  fi
  return $RESULT
}

if [ "$COMMAND" == "stop" ]; then
  core_stop
  if [ "$?" -ne 0 ]; then
    echo "Core is not running (or was started outside of this script)"
  fi

  if [ "$EXTERNAL_POLICIES" == "true" ]; then
    pm_stop
    if [ "$?" -ne 0 ]; then
      echo "Policy Server is not running (or was started outside of this script)"
    fi
  fi
elif [ "$COMMAND" == "restart" ]; then
  core_stop
  if [ "$?" -eq 0 ]; then
    core_start
  else
    echo "Core is not running (or was started outside of this script)"
  fi

  if [ "$EXTERNAL_POLICIES" == "true" ]; then
    pm_stop
    if [ "$?" -eq 0 ]; then
      pm_start
    else
      echo "Policy Server is not running (or was started outside of this script)"
    fi
  fi
elif [ "$COMMAND" == "start" ]; then
  core_start
  if [ "$EXTERNAL_POLICIES" == "true" ]; then
    pm_start
  fi
elif [ "$COMMAND" == "kill" ]; then
  core_stop
  pkill -9 -f $CORE_APPLICATION_NAME
  if [ "$?" -eq 0 ]; then
    echo "Killed all lingering instances of SDL Core"
  fi

  if [ "$EXTERNAL_POLICIES" == "true" ]; then
    pm_stop
    pkill -9 -f $PM_APPLICATION_NAME
    if [ "$?" -eq 0 ]; then
      echo "Killed all lingering instances of the Policy Server"
    fi
  fi
else
  echo "usage: daemon.sh [start/restart/stop/kill] [use_sample_policy_manager?]"
fi
