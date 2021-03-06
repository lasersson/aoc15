#!/bin/sh

UTIL_NAME=$1
SRC_FILE=../src/${UTIL_NAME}.cpp
OUTPUT_FILE=$UTIL_NAME

CFLAGS_COMMON="-I../include -DAOC_LINUX -std=c++11"
CFLAGS_DEBUG="-DAOC_DEBUG $CFLAGS_COMMON"
CFLAGS_RELEASE="-O2 -DAOC_RELEASE $CFLAGS_COMMON"
CFLAGS=$CFLAGS_RELEASE

LDFLAGS="-lstdc++ -lm"

mkdir -p build

pushd build > /dev/null
clang $CFLAGS $SRC_FILE $LDFLAGS -o $OUTPUT_FILE
popd > /dev/null
