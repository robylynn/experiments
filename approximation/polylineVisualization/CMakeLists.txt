include (OgreRenderingApplication)
include (CopyOgreResources)

add_executable(polylineVisualization
  main.cpp)

target_link_libraries(polylineVisualization ${Boost_LIBRARIES}
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARIES}
  CGAL
  gmp
  mpfr
  app
  rendering
  geometry)

add_custom_target(copyPolylineVisualizationData ALL
  COMMAND ${CMAKE_COMMAND} -E  copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/data
  ${CMAKE_CURRENT_BINARY_DIR}/data)

add_ogre_rendering_libraries(polylineVisualization)
copy_standard_ogre_resources(polylineVisualization)
