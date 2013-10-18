#!/bin/sh
# Author: Eugene Nikolsky

git_version=$(git log -1 --format="%h")
git_branch=$(git symbolic-ref --short -q HEAD)
git_tag=$(git describe --tags --exact-match 2>/dev/null)

build_time=$(date)
git_branch_or_tag="${git_branch:-${git_tag}}"

info_plist="${BUILT_PRODUCTS_DIR}/${EXECUTABLE_FOLDER_PATH}/Info.plist"
/usr/libexec/PlistBuddy -c "Set :CFBundleVersion '${git_branch_or_tag}-${git_version}'" "${info_plist}"
/usr/libexec/PlistBuddy -c "Set :BuildTime '${build_time}'" "${info_plist}"
