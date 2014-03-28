#/usr/bin/sh

GIT_REPO=$1


if [ "$GIT_REPO" = "" ]
then
    echo "You should setup your git folder with first paraameter"
    echo "Example:"
    echo -e "\t ./initialise_git.sh /home/{USER_NAME}/applink"
    exit
fi
cd $GIT_REPO
echo "Enter first name: " 
read FIRST_NAME
echo "Enter last name: " 
read LAST_NAME

EMAIL=${FIRST_NAME:0:1}$LAST_NAME"@luxoft.com"
echo "Your email is: "$EMAIL
echo "Correct? [yes or no] "

read ans
while [[ ! $ans =~ (YES|Yes|yes|Y|y|NO|No|no|N|n) ]]
do
	echo "You have entered an invalid response. Please try again"
	read ans
done

if [[ $ans =~ (NO|No|no|N|n) ]]
then	
	echo "Enter your email: ";
	read EMAIL
fi

echo "Your git name: " $FIRST_NAME" "$LAST_NAME
echo "Your git email: " $EMAIL

git config user.name "$FIRST_NAME $LAST_NAME"
git config user.email $EMAIL


MESSAGE_DATA="Not a subject for review\n\n\tReason:"
MESSGAGE_FILE_NAME="default_message"

echo -e $MESSAGE_DATA > .git/$MESSGAGE_FILE_NAME 
git config commit.template .git/$MESSGAGE_FILE_NAME 
echo "Done"




