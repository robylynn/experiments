add_library(rendering
  geometryInterop.cpp
  manualObjects.cpp
  prefabs.cpp
  selectionManager.cpp)

include_directories(${PROJECT_SOURCE_DIR}/inc/rendering)
include_directories(${PROJECT_SOURCE_DIR}/inc/geometry)
