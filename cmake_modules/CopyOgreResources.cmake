#Copy over resources to built directory
function(copy_standard_ogre_resources TargetName)
  add_custom_target(${TargetName}OgreResources ALL
  COMMAND ${CMAKE_COMMAND} -E  copy
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins.cfg ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E  copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg
  ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E  copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/assets
  ${PROJECT_BINARY_DIR}/assets)
endfunction()
