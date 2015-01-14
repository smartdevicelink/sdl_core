#!/bin/bash

if [ $OS_TYPE ]; then 
  if [ $OS_TYPE == "LINUX" ]; then
    (cd ../build/test/ && ./test_suit --gtest_output="xml:$1/test_results/test_suit.xml") && ls -l $1/test_results/
    lcov --capture --directory ../build --output-file $1/coverage_draft_one.info
    lcov --remove $1/coverage_draft_one.info /usr/include/\* --output-file $1/coverage_draft_two.info
    lcov --remove $1/coverage_draft_two.info $1/src/thirdPartyLibs/\* --output-file $1/coverage_draft_three.info
    lcov --remove $1/coverage_draft_three.info $1/test/\* --output-file $1/coverage.info
    genhtml $1/coverage.info --output-directory $1/out
  fi
fi