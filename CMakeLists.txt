cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
set(CMAKE_CXX_FLAGS "-frounding-math -std=c++11")

# Import some packages usable by all
find_package(Boost 1.5.4 COMPONENTS system REQUIRED)
find_package(Glog REQUIRED)

add_subdirectory(smoothing)
