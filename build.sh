rm -r build
mkdir build
cd build
cmake ..
make -j8
framework/tests/representationsTest
averaging/tests/averagingTest
