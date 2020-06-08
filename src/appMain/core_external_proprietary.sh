#!/bin/bash
DIR=$(dirname $0)

EXTERNAL_POLICIES=true
# Allow for manual override to disable sample policy server
if [ "$2" = "false" ]; then
  EXTERNAL_POLICIES=false
fi
$DIR/daemon.sh $1 $EXTERNAL_POLICIES
