#!/usr/bin/env bash

ret=$(git blame -c -L $2,$2 $1  2> /dev/null | awk '{print $2 " " $3}' | awk '{print substr($1,2) $2; }')

echo -n $ret :

