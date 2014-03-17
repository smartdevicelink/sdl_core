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
echo "enter first name" 
read FIRST_NAME
echo "enter last name" 
read LAST_NAME

EMAIL=${FIRST_NAME:0:1}$LAST_NAME"@luxoft.com"
echo "Your email is: "$EMAIL
echo "Correct? [yes or no] "

read answer

while [[ $answer != yes && $answer != no ]]
do
	echo you have entered an invalid response. Please try again
	read answer
done

if [ $answer != yes ] 
then	
	echo "Enter your domain ";
	read EMAIL
fi

git config user.name $FIRST_NAME" "$LAST_NAME
echo git config user.email $EMAIL


MESSAGE_DATA="Reason:\n\nNot a subject for review"
MESSAGE_DATA="Reason:\n\nNot a subject for review"
MESSGAGE_FILE_NAME="default_message"

echo -e $MESSAGE_DATA > .git/$MESSGAGE_FILE_NAME 
git config commit.template .git/$MESSGAGE_FILE_NAME 
echo "Done"




