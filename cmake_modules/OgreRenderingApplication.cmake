find_package(OGRE REQUIRED)
find_package(OIS REQUIRED)
find_package(CEGUI REQUIRED COMPONENTS OGRE)

function(add_ogre_rendering_includes TargetName)
  target_include_directories(${TargetName} PUBLIC ${ROOT_DIR}/framework/inc/app)
  target_include_directories(${TargetName} PUBLIC ${ROOT_DIR}/framework/inc/rendering)
  target_include_directories(${TargetName} PUBLIC ${OGRE_INCLUDE_DIRS})
  target_include_directories(${TargetName} PUBLIC ${OIS_INCLUDE_DIRS})
  target_include_directories(${TargetName} PUBLIC ${CEGUI_INCLUDE_DIR})
  target_include_directories(${TargetName} PUBLIC
    ${CEGUI_OgreRenderer_INCLUDE_DIRS})
endfunction()

function(add_ogre_rendering_libraries TargetName)
  target_link_libraries(${TargetName} ${OGRE_LIBRARIES}
    ${OIS_LIBRARIES}
    ${CEGUI_LIBRARY}
    ${CEGUI_OgreRenderer_LIBRARIES})
endfunction()
