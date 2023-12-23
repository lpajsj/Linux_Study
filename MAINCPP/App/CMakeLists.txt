include_directories(${PROJECT_SOURCE_DIR})
add_library(libmain_task main_task.cpp)
set_target_properties(libmain_task PROPERTIES OUTPUT_NAME "main_task")