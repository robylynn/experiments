rm -r build
set -e
mkdir build
cd build
cmake ..
make -j8
(cd framework/tests; ./frameworkTest)
(cd averaging/tests; ./averagingTest)
