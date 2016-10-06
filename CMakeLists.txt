cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
set(ROOT_DIR "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS "-frounding-math -std=c++11")
add_definitions(-DBOOST_RESULT_OF_USE_DECLTYPE)

# Import some packages usable by all
find_package(Boost 1.5.4 COMPONENTS system REQUIRED)
find_package(Glog REQUIRED)

# Also add some include directories that are usable by all
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(framework)

add_subdirectory(averaging)
