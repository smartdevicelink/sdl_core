#!/bin/bash
# PROJECT_ROOT enviroment variable should contain path
#              to root directory of sources
# NEED_TO_REINSTALL enviroment variable tels if need to reinstall force
# NEED_TO_REBUILD  enviroment variable tels if need to rebuild force

[ -z "$PROJECT_ROOT" ] && PROJECT_ROOT="."

if [ -z "$BINARY_DIR" ]; then
        echo "BINARY_DIR is mandatory"
	exit 1
fi

if [ -z "$PROTOC_BUILD_PATH" ]; then
        echo "PROTOC_BUILD_PATH is mandatory"
	exit 1
fi

if [ -z "$PATH_TO_PROTOBUF_REPO" ]; then
        echo "PATH_TO_PROTOBUF_REPO is mandatory"
	exit 1
fi

[ -z "$NEED_TO_REBUILD" ] && NEED_TO_REBUILD="NO"

function compile_protoc {
	if [[ -d "$PROTOC_BUILD_PATH" ]] && [[  "$(ls -A $PATH_TO_PROTOBUF_BUILD)" ]] ; then
		echo "Protoc already compiled in $PROTOC_BUILD_PATH"
		if [[ "$NEED_TO_REBUILD" ==  "NO" ]]; then
			return 0;
		else
			echo "Rebuild protoc"
		fi
	fi
	echo "Compile protobuf in "$PROTOC_BUILD_PATH
	rm -rf $PROTOC_BUILD_PATH
	mkdir -p $PROTOC_BUILD_PATH
	cd $PROTOC_BUILD_PATH
	$PATH_TO_PROTOBUF_REPO/configure
	make
}

compile_protoc
export LD_LIBRARY_PATH=$PROTOC_BUILD_PATH/src/.libs:$LD_LIBRARY_PATH
