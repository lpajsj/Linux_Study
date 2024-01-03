include_directories(${PROJECT_SOURCE_DIR}/../inc)
add_library(liblog lpajsj_log.c)
set_target_properties(liblog PROPERTIES OUTPUT_NAME "log")