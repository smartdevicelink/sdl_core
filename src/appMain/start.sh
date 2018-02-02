#!/bin/bash
DIR=$(dirname $(type -p $0))
PIDFile=/var/run/sdl.pid
function sdl_start
{
	cd $DIR
	LD_LIBRARY_PATH=$DIR ${DIR}/smartDeviceLinkCore &
	PID=$!
	echo $PID > $PIDFile
}

function sdl_stop
{
	kill $(cat $PIDFile)
	rm $PIDFile
}

if [ x$1 == xstop ]; then
	sdl_stop
elif [ x$1 == xrestart ]; then
	sdl_stop
	sdl_start
else
	sdl_start
fi
