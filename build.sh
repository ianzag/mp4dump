#!/bin/bash

BUILD_TYPE=Release
PROJECT_PATH=${PWD}
BUILD_PATH=${PROJECT_PATH}/build/$BUILD_TYPE
OUTPUT_PATH=${PROJECT_PATH}/output/$BUILD_TYPE

echo "Build type: $BUILD_TYPE"
echo "Project path: $PROJECT_PATH"
echo "Build path: $BUILD_PATH"
echo "Output path: $OUTPUT_PATH"

mkdir -p "$BUILD_PATH"
mkdir -p "$OUTPUT_PATH"

cd "$BUILD_PATH"
cmake \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DEXECUTABLE_OUTPUT_PATH=${OUTPUT_PATH} \
    ${PROJECT_PATH}

cd "$PROJECT_PATH"
ninja -C "$BUILD_PATH"
