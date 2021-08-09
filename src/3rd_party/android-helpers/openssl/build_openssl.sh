#!/bin/bash \

ANDROID_NDK=$1
API_LEVEL=$(echo $2 | tr -dc '0-9')
BUILD_DIR=$3
OUT_DIR=$4

##### export ndk directory. Required by openssl-build-scripts #####
export ANDROID_NDK

##### build-function #####
build_the_thing() {
    echo ---build for android $API_LEVEL
    TOOLCHAIN=$ANDROID_NDK/toolchains/llvm/prebuilt/linux-x86_64
    export PATH=$TOOLCHAIN/$TRIBLE/bin:$TOOLCHAIN/bin:"$PATH"
    ./Configure $SSL_TARGET $OPTIONS -fuse-ld="$TOOLCHAIN/$TRIBLE/bin/ld" && \
    make -j4 && \
    make install DESTDIR=$DESTDIR || exit 128
}

##### set variables according to build-tagret #####
case $5 in
armeabi)
    TRIBLE="arm-linux-androideabi"
    TC_NAME="arm-linux-androideabi-4.9"
    OPTIONS="--target=armv5te-linux-androideabi -mthumb -fPIC -latomic -D__ANDROID_API__=$API_LEVEL"
    DESTDIR="$BUILD_DIR/armeabi"
    SSL_TARGET="android-arm"
    build_the_thing
;;
armeabi-v7a)
    TRIBLE="arm-linux-androideabi"
    TC_NAME="arm-linux-androideabi-4.9"
    OPTIONS="--target=armv7a-linux-androideabi -Wl,--fix-cortex-a8 -fPIC -D__ANDROID_API__=$API_LEVEL"
    DESTDIR="$BUILD_DIR/armeabi-v7a"
    SSL_TARGET="android-arm"
    build_the_thing
;;
x86)
    TRIBLE="i686-linux-android"
    TC_NAME="x86-4.9"
    OPTIONS="-fPIC -D__ANDROID_API__=${API_LEVEL}"
    DESTDIR="$BUILD_DIR/x86"
    SSL_TARGET="android-x86"
    build_the_thing
;;
x86_64)
    TRIBLE="x86_64-linux-android"
    TC_NAME="x86_64-4.9"
    OPTIONS="-fPIC -D__ANDROID_API__=${API_LEVEL}"
    DESTDIR="$BUILD_DIR/x86_64"
    SSL_TARGET="android-x86_64"
    build_the_thing
;;
arm64-v8a)
    TRIBLE="aarch64-linux-android"
    TC_NAME="aarch64-linux-android-4.9"
    OPTIONS="-fPIC -D__ANDROID_API__=${API_LEVEL}"
    DESTDIR="$BUILD_DIR/arm64-v8a"
    SSL_TARGET="android-arm64"
    build_the_thing
;;
esac

#### copy libraries and includes to output-directory #####
mkdir -p $OUT_DIR/include/openssl
cp -R $DESTDIR/usr/local/include/* $OUT_DIR/include
cp -R $DESTDIR/usr/local/lib/*.so $OUT_DIR/lib