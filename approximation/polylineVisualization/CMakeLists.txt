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

add_ogre_rendering_libraries(polylineVisualization)
copy_standard_ogre_resources(polylineVisualization)
