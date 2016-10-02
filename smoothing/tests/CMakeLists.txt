set(REPRESENTATIONS_TEST_SOURCE_FILES "polyloopTest.cpp")

find_package(GTest REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

add_executable(representationsTest main.cpp
  ${REPRESENTATIONS_TEST_SOURCE_FILES}
  )

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()

target_link_libraries(representationsTest ${GTEST_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  CGAL)

