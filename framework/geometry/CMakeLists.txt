add_library(geometry
  cuboidGeometryProvider.cpp
  polyloopBuilder.cpp
  uniformVoxelGrid.cpp)

include_directories(${PROJECT_SOURCE_DIR}/inc/geometry)
