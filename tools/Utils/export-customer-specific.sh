#! /usr/bin/env bash

shopt -s extglob

customer=$1

function is_excluded() {
  for entry in $exclude; do
    if [[ $1 == ${entry%%/} ]]; then
      return 0
    fi
  done
  for entry in $exclude_filenames; do
    if [[ ${1##*/} == $entry ]]; then
      return 0
    fi
  done
  return 1
}

function is_to_filter() {
  for entry in @($files_to_filter); do
    if [[ ${1%%*/} == ${entry} ]]; then
      return 0
    fi
  done
  return 1
}

testme="*.sh \
        PrepareCodeForCustomer"

if [ -z $customer ]; then
  echo "Usage: $0 <customer name>" >&2
  exit 1
fi

case $customer in
  "PASA")
    source ./customer-specific/pasa.conf
  ;;
  "FORD")
    source ./customer-specific/ford.conf
  ;;
esac

if [ -a $export_dir ]; then
  echo "$export_dir already exists" > /dev/stderr
  exit 1
fi

mkdir $export_dir

function integrate() {
  if is_excluded $1; then 
    return
  fi
  echo "$1"
  if [ -d $1 ]; then
    mkdir -p $export_dir/$1
    for l in $1/*; do
      integrate $l;
    done
  elif [ -f $1 ]; then
    if is_to_filter $1; then
      $filter_command $1  > $export_dir/$1
    else
      cp $1 $export_dir/$1
    fi
  fi
}

set -f
for entry in $include; do
  set +f
  for p in $entry; do
    integrate $p
  done
done
