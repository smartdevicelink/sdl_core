#!/bin/bash

# All the branches excluding release branches and virtual HEAD branch
ALL_BRANCHES=`git branch -r | grep -v '\(HEAD\|origin/release\)'`

# Branch names, access dates and last commiter names
# The format is "<unix-date>  <oldness-text>  <author>  <branch-name>"
# Soreted by date, freshes branches first
BRANCH_COMMITS=$(for branch in $ALL_BRANCHES;do echo -e $(git show --format="%at\t%ar\t%aE\t" $branch | head -n 1)$branch; done)
BRANCH_COMMITS=$(echo "$BRANCH_COMMITS"|sort -rn)

# List of all branch owners
BRANCH_OWNERS=$(echo "$BRANCH_COMMITS"|cut -f3|sort|uniq)


for OWNER in $BRANCH_OWNERS; do
  # Oldness and names of branches which belong to $OWNER and that are more than 2 weeks old
  OLD_BRANCHES=$(echo "$BRANCH_COMMITS"|grep $OWNER|grep -v 'days ago'|cut -f 2,4)
  if [ -n "$OLD_BRANCHES" ];
  then
    cat <<EOF
Dear $OWNER, here are your branches you didn't update for more than two weeks,
please merge them to develop and/or remove:
$OLD_BRANCHES

EOF
  fi
done

