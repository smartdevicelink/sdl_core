#!/bin/bash

pipe=/tmp/test_DnssdServiceDiscovery
mkfifo $pipe
avahi-publish -s TestService _ford-sdlapp._tcp 4444 >$pipe 2>&1 &
pid=$!
read line <$pipe

ret=0
if [[ "$line" == Established* ]]
then
  ./test_DnssdServiceDiscovery
  ret=$?
else
  ret=3
fi

kill $pid
rm $pipe
exit $ret

