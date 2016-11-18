find_package(Eigen REQUIRED)

set(SIMPLIFICATION_SOURCE_FILES
  "simplification/polyline_3DouglasPeucker.cpp"
  "simplification/polyline_3NaiveCircle.cpp")

add_library(geometry
  cuboidGeometryProvider.cpp
  polylineBuilder.cpp
  polyloopBuilder.cpp
  polyloop2Builder.cpp
  uniformVoxelGrid.cpp)

add_library(geometry_algorithms
  ${SIMPLIFICATION_SOURCE_FILES})

# Generic polyline simplification library.
target_include_directories(geometry_algorithms PUBLIC
  ${ROOT_DIR}/ext/geometry/psimpl)
# TODO msati3: Currently, the geometry library has a dependency that has
# crept in due to VertexElement class. Current work in progress in feature
# branch is aimed at removing this. This include will then disappear.
target_include_directories(geometry_algorithms PUBLIC
  ${PROJECT_SOURCE_DIR}/inc/rendering)
target_include_directories(geometry_algorithms PUBLIC
  ${EIGEN_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/inc/geometry)
