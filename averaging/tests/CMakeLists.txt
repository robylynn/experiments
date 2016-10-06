set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

add_executable(averagingTest
  main.cpp
  distanceFieldTest.cpp)

include_directories(${PROJECT_SOURCE_DIR}/inc/geometry)
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

target_link_libraries(averagingTest ${GTEST_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  CGAL
  geometry
  averagingLib)

