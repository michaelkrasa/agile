#!/bin/bash

# This script will ensure the starting files are up-to-date in the current branch 
# 
# It does this by fetching the upstream repository (the one we control) and rebasing
# on top of it. If you have already modified these starting files then you may need
# to resolve some conflicts. If you do need to resolve conflicts you will need to
# force push afterwards.
#
# You should run this script at the start of an exercise and if told to do so. 
#
# Author: Giles Reger

# find tag name
BRANCH=$(git branch | grep "\*" | cut -d ' ' -f 2)

# The remote upstream should be added in your repository 
if ! git config remote.upstream.url > /dev/null; then
  git remote add upstream https://gitlab.cs.man.ac.uk/mbaxjgr2/comp26120_base
fi

# Get any upstream changes
git fetch upstream

# Rebase on top of them (apply them before any local changes)
git rebase upstream/"${BRANCH}"

# This will diverge from origin so force push
git push -f origin "${BRANCH}"
