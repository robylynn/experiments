#Copy over resources to built directory
function(copy_standard_ogre_resources TargetName)
  add_custom_command(TARGET ${TargetName} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy
  ${CMAKE_CURRENT_SOURCE_DIR}/plugins.cfg ${PROJECT_BINARY_DIR})
add_custom_command(TARGET ${TargetName} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg
  ${PROJECT_BINARY_DIR})
add_custom_command(TARGET ${TargetName} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  make_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/materials)
add_custom_command(TARGET ${TargetName} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E  copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/materials
  ${PROJECT_BINARY_DIR}/materials)
  message(${CMAKE_CURRENT_SOURCE_DIR})
endfunction()
