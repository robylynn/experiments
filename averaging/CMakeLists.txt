include(OgreRenderingApplication)
include(CopyOgreResources)

project(averaging)

# TODO msati3: Going forward, split windowing component into library

add_executable(averaging
  main.cpp
  viewManager.cpp
  averagingLinesView.cpp
  averagingPolyloopsView.cpp
  averagingVectorsView.cpp
  commonViewInteractionsHandler.cpp)

add_library(averagingLib
  separableGeometryInducedField.cpp)

include_directories(${ROOT_DIR}/framework/inc/app)
include_directories(${ROOT_DIR}/framework/inc/geometry)
include_directories(${ROOT_DIR}/framework/inc/rendering)
include_directories(${ROOT_DIR}/framework/inc/utils)
include_directories(${ROOT_DIR}/averaging)

link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(averaging ${Boost_LIBRARIES}
  ${GLOG_LIBRARIES}
  ${GFLAGS_LIBRARIES}
  ${CGAL_LIBS}
  app
  geometry
  rendering
  averagingLib)

add_ogre_rendering_libraries(averaging)
copy_standard_ogre_resources(averaging)

# Copy data folder
add_custom_command(TARGET averaging POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/data
  ${CMAKE_CURRENT_BINARY_DIR}/data)

add_subdirectory(tests)
