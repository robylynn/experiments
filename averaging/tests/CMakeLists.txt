set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(averagingTest
  main.cpp
  separableGeometryInducedFieldTest.cpp)

include_directories(${PROJECT_SOURCE_DIR}/inc/geometry)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

target_link_libraries(averagingTest ${GTEST_LIBRARIES}
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  ${CGAL_LIBS}
  geometry
  averagingLib)

