add_library(geometry
  #cuboidGeometryProvider.cpp
  polyline_3Builder.cpp
  polyloop_3Builder.cpp
  polyloop2Builder.cpp
  #uniformVoxelGrid.cpp
  )

find_package(Eigen REQUIRED)

include_directories(${EIGEN_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/inc/geometry)
