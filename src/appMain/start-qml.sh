#!/bin/sh

#
# This script start qml on QNX
#

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:/usr/lib:
QWS_DISPLAY=qnx
QWS_MOUSE_PROTO=qnx
QWS_KEYBOARD=qnx
export QWS_DISPLAY QWS_MOUSE_PROTO QWS_KEYBOARD

if [ ` pidin | grep devi-hid | wc -l` -eq 0 ]
then
  slay -f devi-hid
fi

filename=$1
pluginpath=$2

if [ -z "$filename" ]
then
  echo "Usage: start-qml.sh <filename> [<pluginpath>]"
  exit 1
fi
if [ -n "$pluginpath" ]
then
  path="-I $pluginpath"
fi

/usr/photon/bin/devi-hid -Pr kbd mouse
qmlviewer -fullscreen -stayontop -qws $filename $path
slay devi-hid
