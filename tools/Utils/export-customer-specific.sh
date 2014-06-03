#! /usr/bin/env bash

shopt -s extglob

srcdir=$1
bdldir=$2
customer=$3

function find_in_excludes() {
	text=$1
	upper=$(echo ${#excludes[@]})
	buttom=0
	while [ $buttom -le $upper ]
	do
		pivot=$(($buttom+($upper-$buttom)/2))
		cur=${excludes[$pivot]}
		if [[ $text < $cur ]]
		then
			upper=$(($pivot-1))
		elif [[ $text > $cur ]]
		then
			buttom=$(($pivot+1))
		else
			result=$pivot
			return 0
		fi
	done	
	result="error"
	return 1
}


function is_excluded() {
  find_in_excludes $1
  if [[ $result -eq "error" ]]
  then 
	return 1
  fi
  
  for entry in $exclude_filenames; do
    if [[ ${1##*/} == $entry ]]; then
      return 1
    fi
  done
  return 0
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
  echo "Usage: $0 <source dir> <build dir> <customer name>" >&2
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
readarray -t excludes < <(for entry in $exclude; do echo $entry; done | sort)

if [ -a $export_dir ]; then
  echo "$export_dir already exists" > /dev/stderr
  exit 1
fi

mkdir $export_dir

function integrate() {
  relfn=${1##$2/}
  if is_excluded $relfn; then 
    return
  fi
  echo "$relfn"
  if [ -d $1 ]; then
    mkdir -p $export_dir/$relfn
    for l in $1/*; do
      integrate $l $2;
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
for entry in $include_src; do
  set +f
  for p in $srcdir/$entry; do
    integrate $p $srcdir
  done
done


set -f
for entry in $include_bld; do
  set +f
  for p in $bdldir/$entry; do
    integrate $p $bdldir
  done
done

cp -r $specificdir/* $export_dir

find $export_dir \
  -regex '.*\.\(cc\|json\|h\|cpp\|ini\|hpp\|txt\|html\|properties\|cfg\|sql\|sh\|py\|pl\)' \
  -exec unix2dos {} \;
