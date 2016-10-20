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

add_subdirectory(polylineVisualization)
