#/usr/bin/sh

GIT_REPO=$1
MESSAGE_DATA="Not a subject for review"
MESSGAGE_FILE_NAME="default_message"
GIT_CONFIG_DATA="[commit]\n\ttemplate = .git/$MESSGAGE_FILE_NAME"

if [ "$GIT_REPO" = "" ]
then
    echo "You should setup your git folder with first paraameter"
    echo "Example:"
    echo -e "\t ./initialise_git.sh /home/{USER_NAME}/applink"    
    exit
fi

cd $GIT_REPO/.git 
echo $MESSAGE_DATA > $MESSGAGE_FILE_NAME 
echo $GIT_CONFIG_DATA >> ./config 
echo "Done"
