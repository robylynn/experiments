project(smoothing)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ../cmake_modules)

find_package(Boost 1.5.4 COMPONENTS system REQUIRED)
find_package(OGRE REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "-frounding-math -std=c++11")

add_executable(smoothing main.cpp)

MESSAGE("Boost library location is ${BOOST_LIBRARYDIR}")

target_link_libraries(smoothing ${Boost_LIBRARIES}
                                ${OGRE_LIBRARIES}
                                CGAL)

