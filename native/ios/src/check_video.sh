#!/bin/bash
# Author: Eugene Nikolsky

video_dir="../../../web/src/video"

if echo "$TARGET_NAME" | grep -qi french
then
    video_dir="../../../web/src/fr/video"
fi

# create the directory just in case
mkdir -p "$video_dir"

if [ ! "$( ls $video_dir )" ]
then
    echo "Warning! The video directory $video_dir has no files!"
    exit 1
fi

