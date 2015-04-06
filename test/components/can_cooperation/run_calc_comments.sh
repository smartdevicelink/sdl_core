#!/bin/sh
# Script to call awk line-counting script

show_help() {
  echo "
        Usage: ./run_calc_comments.sh [OPTION]...
        Counts comment percentage in source files.\n
        Arguments:
        -h    displays help message and exits
        -d PATH_TO_DIRECTORY specifies path to directory with source files.\
        Script applies counting to all files in directory recursively. \
        Default is current directory.
        -f FILE_FORMAT  specifies file extension or other format of file names. \
        Script applies counting to all files with specified file name recursively in directory. \
        Default is *.h.\n
        Example:
        ./run_calc_comments.sh -f *.h -d ../../../src/components/can_cooperation/\n"
}


OPTIND=1
input_directory="."
file_type="*.h"

while getopts "hd:f:" opt; do
    case "$opt" in
    h)
        show_help
        exit 0
        ;;
    d)  input_directory=$OPTARG
        ;;
    f)  file_type=$OPTARG
        ;;
    esac
done

find $input_directory -name "$file_type" -printf "%p " | xargs awk -f ./lc.awk