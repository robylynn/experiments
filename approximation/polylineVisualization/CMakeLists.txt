add_executable(polylineVisualization
  main.cpp)

target_link_libraries(polylineVisualization ${Boost_LIBRARIES}
  ${OGRE_LIBRARIES}
  ${GLOG_LIBRARIES}
  ${OIS_LIBRARIES}
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARIES}
  CGAL
  gmp
  mpfr
  app
  rendering
  geometry)

# TODO msati3: Remove this from all CMakefiles and place in a common location.
# Copy over OGRE plugins file to built directory
add_custom_command(TARGET polylineVisualization POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins.cfg ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET polylineVisualization POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg
  ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET polylineVisualization POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  make_directory
  ${CMAKE_CURRENT_BINARY_DIR}/materials)
add_custom_command(TARGET polylineVisualization POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/materials
  ${CMAKE_CURRENT_BINARY_DIR}/materials)
