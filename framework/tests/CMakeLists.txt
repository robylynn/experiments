set(GEOMETRY_TEST_SOURCE_FILES
  "geometry/cuboidTest.cpp"
  "geometry/polyloopTest.cpp"
  "geometry/uniformVoxelGridTest.cpp")

set(UTILS_TEST_SOURCE_FILES
  "utils/variantWrapper.cpp")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(frameworkTest
  main.cpp
  ${GEOMETRY_TEST_SOURCE_FILES}
  ${UTILS_TEST_SOURCE_FILES}
  )

include_directories(${PROJECT_SOURCE_DIR}/inc/geometry)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

target_link_libraries(frameworkTest ${GTEST_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  CGAL
  geometry)

