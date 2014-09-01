#!//bin/bash
last_develop=`git --git-dir=$1  log origin/develop| head -n 1|cut -f 2 -d ' '`;
git --git-dir=$1  show --pretty="format:  %an| %ae | %H | %s" --name-only $last_develop
#a=$()
#echo $a