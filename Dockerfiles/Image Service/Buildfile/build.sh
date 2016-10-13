#! /bin/bash

if [ -z "$BUILD_TYPE" ]; then
	export BUILD_TYPE=Release
fi

echo "Making $BUILD_TYPE..."
mkdir -p Image\ Service/build
cd Image\ Service/build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
make

cd ../../
mkdir -p dependencies/build/libraries
cp /usr/local/lib/libboost_system.so.1.61.0 dependencies/build/libraries/
cp /usr/local/lib/libvips-cpp.so.42.6.1 dependencies/build/libraries/
cp /usr/local/lib/libvips.so.42.6.1 dependencies/build/libraries/
cp Image\ Service/build/run dependencies/build/image_service