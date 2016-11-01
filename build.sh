set -e
rm -r build
mkdir build
cd build
cmake ..
make -j8
(cd framework/tests; ./frameworkTest)
(cd averaging/tests; ./averagingTest)
