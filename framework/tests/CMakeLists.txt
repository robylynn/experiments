set(GEOMETRY_TEST_SOURCE_FILES
  "geometry/cuboidTest.cpp"
  "geometry/polyloopTest.cpp"
  "geometry/triangleMeshTest.cpp"
  "geometry/uniformPlanarGridTest.cpp"
  "geometry/uniformVoxelGridTest.cpp")

set(UTILS_TEST_SOURCE_FILES
  "utils/variantWrapperTest.cpp")

set(RENDERING_TEST_SOURCE_FILES
  "rendering/positionOnlyBufferProviderTest.cpp")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(frameworkTest
  main.cpp
  ${GEOMETRY_TEST_SOURCE_FILES}
  ${UTILS_TEST_SOURCE_FILES}
  ${RENDERING_TEST_SOURCE_FILES}
  )

#TODO msati3: Split into multiple executables with contained dependencies?
include_directories(${PROJECT_SOURCE_DIR}/inc/geometry)
include_directories(${PROJECT_SOURCE_DIR}/inc/rendering)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

target_link_libraries(frameworkTest ${GTEST_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  CGAL
  gmp
  mpfr
  geometry)

