add_library(rendering
  prefabs.cpp
  selectionManager.cpp
  manualObjects.cpp)

include_directories(${PROJECT_SOURCE_DIR}/inc/rendering)
include_directories(${PROJECT_SOURCE_DIR}/inc/geometry)
