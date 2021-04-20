#!/bin/bash
trap close INT

function close() {
  $DIR/core.sh stop
}

DIR=$(dirname $0)
CORE_PID_FILE=${DIR}/core.pid

$DIR/core.sh start

# Wait for the application to close
CORE_PID=$(cat $CORE_PID_FILE)
tail --pid=$CORE_PID -f /dev/null

# Verify that the application closed sucessfully
close > /dev/null
