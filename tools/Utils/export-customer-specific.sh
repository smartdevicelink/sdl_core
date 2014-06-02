#! /usr/bin/env bash

shopt -s extglob

srcdir=$1
customer=$2

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

if [ -z $srcdir ] ||  [ -z $customer ]; then
  echo "Usage: $0 <source dir> <customer name>" >&2
  exit 1
fi

case $customer in
  "PASA")
    source $srcdir/customer-specific/pasa.conf
    specificdir=$srcdir/customer-specific/pasa
  ;;
  "FORD")
    source $srcdir/customer-specific/ford.conf
    specificdir=$srcdir/customer-specific/ford
  ;;
esac

if [ -a $export_dir ]; then
  echo "$export_dir already exists" > /dev/stderr
  exit 1
fi

mkdir $export_dir

function integrate() {
  relfn=${1##$srcdir/}
  if is_excluded $relfn; then 
    return
  fi
  echo "$relfn"
  if [ -d $1 ]; then
    mkdir -p $export_dir/$relfn
    for l in $1/*; do
      integrate $l;
    done
  elif [ -f $1 ]; then
    if is_to_filter $relfn; then
      $srcdir/$filter_command $1  > $export_dir/$relfn
    else
      cp $1 $export_dir/$relfn
    fi
  fi
}

set -f
for entry in $include; do
  set +f
  for p in $srcdir/$entry; do
    integrate $p
  done
done

cp -r $specificdir/* $export_dir

find $export_dir \
  -regex '.*\.\(cc\|json\|h\|cpp\|ini\|hpp\|txt\|html\|properties\|cfg\|sql\|sh\|py\|pl\)' \
  -exec unix2dos {} \;
