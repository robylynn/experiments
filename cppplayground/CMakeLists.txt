project(cppplayground)

# TODO msati3: Going forward, split windowing component into library
add_executable(cppplayground
  main.cpp)

include_directories(${ROOT_DIR}/framework/inc/utils)
include_directories(${ROOT_DIR}/framework/inc/geometry)

link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(cppplayground ${Boost_LIBRARIES}
  ${GLOG_LIBRARIES}
  ${CGAL_LIBS})

