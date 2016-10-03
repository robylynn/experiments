project(framework)

# Utils may be used throughout the framework
include_directories(${PROJECT_SOURCE_DIR}/inc/utils)

add_subdirectory(app)
add_subdirectory(geometry)
add_subdirectory(rendering)

add_subdirectory(tests)
