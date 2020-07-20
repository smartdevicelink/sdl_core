#!/bin/bash
DIR=$(dirname $0)

EXTERNAL_POLICIES=false
if [ "$2" = "true" ]; then
  EXTERNAL_POLICIES=true
fi
$DIR/daemon.sh $1 $EXTERNAL_POLICIES
