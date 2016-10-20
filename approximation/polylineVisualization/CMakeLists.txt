include (CopyOgreResources)

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

copy_standard_ogre_resources(polylineVisualization)
