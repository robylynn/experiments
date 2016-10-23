project(approximation)

add_executable(approximation
  main.cpp)

include_directories(${ROOT_DIR}/framework/inc/app)
include_directories(${ROOT_DIR}/framework/inc/geometry)
include_directories(${ROOT_DIR}/framework/inc/rendering)
include_directories(${ROOT_DIR}/framework/inc/utils)
include_directories(${ROOT_DIR}/approximation)

link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(approximation ${Boost_LIBRARIES}
  ${OIS_LIBRARIES}
  CGAL
  gmp
  mpfr
  app
  geometry)

add_subdirectory(polylineVisualization)
