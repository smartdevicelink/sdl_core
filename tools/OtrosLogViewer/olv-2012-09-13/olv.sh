#!/bin/bash
 ###############################################################################
 # Copyright 2011 Krzysztof Otrebski
 # 
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 # 
 #   http://www.apache.org/licenses/LICENSE-2.0
 # 
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 ###############################################################################

# based on http://stackoverflow.com/questions/59895/can-a-bash-script-tell-what-directory-its-stored-in

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ] ; do SOURCE="$(readlink "$SOURCE")"; done
CURRENTDIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

#cd $CURRENTDIR

MEMORY=-Xmx1024m
LOG_PROPERTIES=-Djava.util.logging.config.file=logging.properties
#SFTP_KEY=-Dvfs.Identities=

if [ -n "$JAVA_HOME" ]
then
 JAVA="$JAVA_HOME/bin/java"
else
 JAVA=java
fi


if [  "-batch" = "$1" ]
then
 $JAVA $LOG_PROPERTIES $MEMORY $SFTP_KEY -jar "$CURRENTDIR/lib/OtrosStarter.jar" $@
else
 # we need to be sure we are writing to a directory where we have write access, /tmp is one of them but app directory is clearly not
 $JAVA $LOG_PROPERTIES $MEMORY $SFTP_KEY -jar "$CURRENTDIR/lib/OtrosStarter.jar" $@ > /tmp/olv.log 2>&1
fi


