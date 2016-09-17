project(smoothing)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ../cmake_modules)

find_package(Boost 1.5.4 COMPONENTS system REQUIRED)
find_package(OGRE REQUIRED)
find_package(Glog REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "-frounding-math -std=c++11")

add_executable(smoothing main.cpp)

#MESSAGE("Google log location is ${GLOG_LIBRARY}")

target_link_libraries(smoothing ${Boost_LIBRARIES}
                                ${OGRE_LIBRARIES}
                                ${GLOG_LIBRARIES}
                                CGAL)

add_custom_command(TARGET smoothing POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E  copy
                   ${CMAKE_CURRENT_SOURCE_DIR}/plugins.cfg ${PROJECT_BINARY_DIR})

