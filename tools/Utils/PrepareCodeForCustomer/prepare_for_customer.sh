#! /bin/sh

CUSTOMER=$1
REPO=$2
if [ "$CUSTOMER" = "" ]
then
    echo "You should setup customer name in UPPERCASE as first arg"
    exit
fi
if [ "$REPO" = "" ]
then
    echo "You should setup path to repository as second arg"
    exit
fi

find $REPO | grep -E "(.\.c$)|(.\.cc$)|(.\.h&)|(.\.cpp$)"|xargs python ./main.py $CUSTOMER |grep Changes

