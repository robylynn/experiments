project(approximation)

# TODO msati3: Going forward, split windowing component into library
find_package(OGRE REQUIRED)
find_package(OIS REQUIRED)

add_executable(approximation
  main.cpp)

#add_library(averagingLib
#  separableGeometryInducedField.cpp)

include_directories(${ROOT_DIR}/framework/inc/app)
include_directories(${ROOT_DIR}/framework/inc/geometry)
include_directories(${ROOT_DIR}/framework/inc/rendering)
include_directories(${ROOT_DIR}/framework/inc/utils)
include_directories(${ROOT_DIR}/approximation)

link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(approximation ${Boost_LIBRARIES}
  ${OGRE_LIBRARIES}
  ${GLOG_LIBRARIES}
  ${OIS_LIBRARIES}
  CGAL
  gmp
  mpfr
  app
  geometry)

# TODO msati3: Remove this from all CMakefiles and place in a common location.
# Copy over OGRE plugins file to built directory
add_custom_command(TARGET approximation POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins.cfg ${PROJECT_BINARY_DIR})
add_custom_command(TARGET approximation POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg
  ${PROJECT_BINARY_DIR})
add_custom_command(TARGET approximation POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  make_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/materials)
add_custom_command(TARGET approximation POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/materials
  ${PROJECT_BINARY_DIR}/materials)

