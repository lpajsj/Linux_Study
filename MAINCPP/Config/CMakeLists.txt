include_directories(${PROJECT_SOURCE_DIR})
add_library(libconfig config.cpp)
set_target_properties(libconfig PROPERTIES OUTPUT_NAME "config")