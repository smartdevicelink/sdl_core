#!/usr/bin/env bash

BUILD_DIR=$1
COVERAGE_DIR=$BUILD_DIR/coverage
REPORTS_DIR=$BUILD_DIR/coverage_reports

if [ "$BUILD_DIR" = "" ]
then
    echo "You should specify your build directory as first argument "
    echo "Example:"
    echo "\t\$ collect_coverage.sh [PATH TO BUILD DIR]"
    exit
fi


rm -rf $COVERAGE_DIR
rm -rf $REPORTS_DIR -

mkdir $COVERAGE_DIR
lcov --quiet --capture --directory . --output-file $COVERAGE_DIR/full_report.info
lcov --quiet --remove $COVERAGE_DIR/full_report.info '/usr/*' '*/test/*' '*/src/3rd*' '*/build/src/*'  --output-file $COVERAGE_DIR/coverage.info

mkdir $REPORTS_DIR
genhtml --quiet $COVERAGE_DIR/coverage.info --output-directory $REPORTS_DIR
html2text -width 150 $REPORTS_DIR/index.html
tar -zcf coverage_report.tar.gz $REPORTS_DIR
echo More information avaliable in $REPORTS_DIR/index.html
