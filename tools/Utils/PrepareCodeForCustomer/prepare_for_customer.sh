#! /bin/sh

if [ "$1" = "-h" ]
then
	echo "Usage:"
	echo "./prepare_for_customers.sh CUSTOMER_NAME PATH_TO_REPO"
	exit
fi

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


find $REPO -regextype posix-egrep -regex '.*\.(c|cc|cpp|h)$' -exec python ./main.py FORD {} + | grep Changes
