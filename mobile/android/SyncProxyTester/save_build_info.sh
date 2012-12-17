#!/bin/sh
# Author: Eugene Nikolsky

git_version=$(git log -1 --format="%h")
git_branch=$(git symbolic-ref --short -q HEAD)

build_time=$(date)

echo "$git_branch-$git_version ($build_time)" > assets/build.info
