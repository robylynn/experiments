include (OgreRenderingApplication)

add_library(app
  cameraController.cpp
  inputManager.cpp
  windowedRenderingApp.cpp)

add_ogre_rendering_includes(app)
include_directories(${PROJECT_SOURCE_DIR}/inc/app)
