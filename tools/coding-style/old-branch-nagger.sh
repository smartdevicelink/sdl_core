#!/bin/bash

# All the branches excluding release branches and virtual HEAD branch
ALL_BRANCHES=`git branch -r | grep -v '\(HEAD\|origin/release\)'`

# Branch names, access dates and last committer names
# The format is "<unix-date>  <oldness-text>  <author>  <branch-name>"
# Sorted by date, freshes branches first
BRANCH_COMMITS=$(for branch in $ALL_BRANCHES;do echo -e $(git log -1 --format="%at\t%ar\t%aE\t" $branch)$branch; done | sort -rn)

# List of all branch owners
BRANCH_OWNERS=$(echo "$BRANCH_COMMITS"|cut -f3|sort|uniq)

# 2 weeks here
TIME_THRESHOLD=$((60 * 60 * 24 * 7 * 2))
DATE_THRESHOLD=$(( $(date +%s) - $TIME_THRESHOLD ))

FLAGS=${1:-""}

for OWNER in $BRANCH_OWNERS; do
  # Oldness and names of branches which belong to $OWNER and that are more than 2 weeks old
  OLD_BRANCHES=$(echo "$BRANCH_COMMITS"|grep $OWNER|awk -F'\t' 'int($1) < '$DATE_THRESHOLD' {print $2 " " $4}')
  if [ -n "$OLD_BRANCHES" ];
  then
    if [[ $FLAGS == "-m" ]]; then
        SENDER="mail -s 'Your branches are too old!' $OWNER"
    else
        SENDER=cat
    fi
    $SENDER <<EOF
Dear $OWNER, here are your branches you didn't update for more than two weeks,
please merge them to develop and/or remove:
$OLD_BRANCHES

EOF
  fi
done

