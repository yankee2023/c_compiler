rm -rf build
rm -rf bin
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
