cmake_minimum_required(VERSION 3.5)
project(cmake_test)
set(test_val "hello world")
message("${test_val}")
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(TOOLCHAIN_DIR /opt/fsl-imx-x11/4.1.15-2.1.0/sysroots)
set(CMAKE_SYSROOT ${TOOLCHAIN_DIR}/cortexa7hf-neon-poky-linux-gnueabi)
SET(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++)
SET(CMAKE_C_FLAGS "-march=armv7ve -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a7 -lc -pthread -O0")
SET(CMAKE_CXX_FLAGS "-march=armv7ve -mfpu=neon -mfloat-abi=hard -mcpu=cortex-a7 -std=c++11 -pthread -O0")
# SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# # SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

# SET(cmake_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
# SET(cmake_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
# add_compile_options("-o0 -Wall -std=c++11") 
# SET(CMAKE_C_FLAGS "-o0 -Wall -m32")
# SET(CMAKE_CXX_FLAGS "-o0 -Wall -std=c++11 -m32")
add_subdirectory(App)
add_subdirectory(Config)
add_subdirectory(Mylib)
include_directories(${PROJECT_SOURCE_DIR}/App ${PROJECT_SOURCE_DIR}/Config ${PROJECT_SOURCE_DIR}/Mylib ${PROJECT_SOURCE_DIR}/Main)
# link_directories(${PROJECT_SOURCE_DIR}/App ${PROJECT_SOURCE_DIR}/Config ${PROJECT_SOURCE_DIR}/Mylib)
add_library(libled STATIC ${PROJECT_SOURCE_DIR}/Main/led.c)
set_target_properties(libled PROPERTIES OUTPUT_NAME "led")
add_executable(hello ${PROJECT_SOURCE_DIR}/Main/main.cpp)
target_link_libraries(hello libmain_task libeth_task libconfig liblog libled pthread )
set_target_properties(hello PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/)
















# cmake_minimum_required(VERSION 3.5)
# project(HELLO)
# set(CMAKE_C_FLAGS "-lpthread")
# set(CMAKE_CXX_FLAGS "-std=c++11")
# add_subdirectory(App)
# add_subdirectory(Config)
# add_subdirectory(Mylib)
# include_directories(${PROJECT_SOURCE_DIR}/App ${PROJECT_SOURCE_DIR}/Config ${PROJECT_SOURCE_DIR}/Mylib)
# # link_directories(${PROJECT_SOURCE_DIR}/App ${PROJECT_SOURCE_DIR}/Config ${PROJECT_SOURCE_DIR}/Mylib)
# add_executable(hello ${PROJECT_SOURCE_DIR}/Main/main.cpp)
# target_link_libraries(hello libmain_task libeth_task libconfig liblog pthread)
# set_target_properties(hello PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/)