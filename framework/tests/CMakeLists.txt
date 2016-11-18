set(GEOMETRY_TEST_SOURCE_FILES
  #"geometry/cuboidTest.cpp"
  "geometry/polyline_3Test.cpp"
  "geometry/polyloop_2Test.cpp"
  "geometry/polyloop_3Test.cpp"
  "geometry/triangleMeshTest.cpp"
  "geometry/circleSamplerTest.cpp"
  #"geometry/uniformPlanarGridTest.cpp"
  #"geometry/uniformVoxelGridTest.cpp"
  )

set(GEOMETRY_ALGORITHMS_TEST_SOURCE_FILES
  "geometry/simplification/polyline_3SimplificationTest.cpp"
  )

set(UTILS_TEST_SOURCE_FILES
  "utils/tupleIteratorTest.cpp"
  "utils/stencilCirculatorIteratorTest.cpp"
  "utils/attributeIteratorTest.cpp"
  "utils/variantWrapperTest.cpp"
  )

set(RENDERING_TEST_SOURCE_FILES
  "rendering/positionOnlyBufferProviderTest.cpp"
  )

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
find_package(Eigen REQUIRED)

add_executable(frameworkTest
  main.cpp
  ${GEOMETRY_TEST_SOURCE_FILES}
  ${UTILS_TEST_SOURCE_FILES}
  ${RENDERING_TEST_SOURCE_FILES}
  ${GEOMETRY_ALGORITHMS_TEST_SOURCE_FILES}
  )

#TODO msati3: Split into multiple executables with contained dependencies?
include_directories(${EIGEN_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/inc/geometry)
include_directories(${PROJECT_SOURCE_DIR}/inc/rendering)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

target_link_libraries(frameworkTest ${GTEST_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  CGAL
  gmp
  mpfr
  geometry
  geometry_algorithms
  ${GLOG_LIBRARIES})

add_custom_command(TARGET frameworkTest POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/data
  ${CMAKE_CURRENT_BINARY_DIR}/data)
