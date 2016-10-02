project(smoothing)

# TODO msati3: Going forward, split windowing component into library
find_package(OGRE REQUIRED)
find_package(OIS REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/geometryPrimitives)
link_directories(${Boost_LIBRARY_DIRS})

add_executable(smoothing main.cpp
  uniformVoxelGrid.cpp
  windowedRenderingApp.cpp)

target_link_libraries(smoothing ${Boost_LIBRARIES}
  ${OGRE_LIBRARIES}
  ${GLOG_LIBRARIES}
  ${OIS_LIBRARIES}
  CGAL)

# Copy over OGRE plugins file to built directory
add_custom_command(TARGET smoothing POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins.cfg ${PROJECT_BINARY_DIR})
add_custom_command(TARGET smoothing POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg
  ${PROJECT_BINARY_DIR})
add_custom_command(TARGET smoothing POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  make_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/materials)
add_custom_command(TARGET smoothing POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/materials
  ${PROJECT_BINARY_DIR}/materials)

add_subdirectory(tests)
