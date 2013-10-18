#!/bin/bash

USAGE=$( cat <<EOF
Usage: ${0} src_videos_dir dst_videos_dir optional_flags

src_videos_dir must point to the 'output_from_Ford_3.5' directory containing
'English' and 'French' subdirectories.

dst_videos_dir must point to the 'web/src' directory of the project.

with optional_flags you can specify some extra flags for rsync (for example,
-n to dry run the process).
EOF
)

set -x

# source and target directories
SRC_VIDEO=${1?"${USAGE}"}
DST_VIDEO=${2?"${USAGE}"}
FLAGS=${3:-""}

RSYNC="rsync"
RSYNC_RENAME_OPTS=${FLAGS}\ --archive\ --verbose\ --checksum\ --inplace\ --progress
# ignore Welcome orientation videos for all platforms
RSYNC_OPTS=${RSYNC_RENAME_OPTS}\ --delete-excluded\ --exclude='FAQ_Welcome_orientation*'\ --include='*.mp4'\ --exclude='*'

WO_SRC_NAME="FAQ_Welcome_orientation_IOS.mp4"
WO_DST_NAME="FAQ_Welcome_orientation.mp4"

${RSYNC} ${RSYNC_OPTS} "${SRC_VIDEO}/English/MP4/" "${DST_VIDEO}/video/"
# copy and rename Welcome Orientation for iOS build
${RSYNC} ${RSYNC_RENAME_OPTS} "${SRC_VIDEO}/English/MP4/${WO_SRC_NAME}" "${DST_VIDEO}/video/${WO_DST_NAME}"

${RSYNC} ${RSYNC_OPTS} "${SRC_VIDEO}/French/MP4/" "${DST_VIDEO}/fr/video/"
# copy and rename Welcome Orientation for iOS build
${RSYNC} ${RSYNC_RENAME_OPTS} "${SRC_VIDEO}/French/MP4/${WO_SRC_NAME}" "${DST_VIDEO}/fr/video/${WO_DST_NAME}"
