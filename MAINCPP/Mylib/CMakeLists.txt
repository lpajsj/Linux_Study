include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/Lib/inc)
add_library(liblog lpajsj_log.c ${PROJECT_SOURCE_DIR}/Lib/src/fifo_buffer.c)
set_target_properties(liblog PROPERTIES OUTPUT_NAME "log")