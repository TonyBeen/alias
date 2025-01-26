cmake_minimum_required(VERSION 3.10)

# 项目信息
project(kcpp C VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)

# default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo") # Release MinSizeRel RelWithDebInfo Debug
endif()

option(EVENT_DYNAMIC_LIB "Build dynamic library" ON)
option(EVENT_STATIC_LIB "Build static library" ON)
option(BUILD_EXAMPLES "Compile sample program" OFF)
option(BUILD_TEST "Compile sample program" OFF)
option(ENABLE_DEBUG "Enable debug symbol" OFF)

if(ENABLE_DEBUG MATCHES "ON")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# 设置库的源文件
set(SOURCES
    src/file1.c
    src/file2.c
    src/file3.c
)

# 生成静态库
add_library(MyStaticLibrary STATIC ${SOURCES})

# 生成动态库
add_library(MySharedLibrary SHARED ${SOURCES})

# 包含头文件目录
target_include_directories(MyStaticLibrary PUBLIC include)
target_include_directories(MySharedLibrary PUBLIC include)

# 设置版本号
set_target_properties(MySharedLibrary PROPERTIES VERSION 1.0 SOVERSION 1)


# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)
endif()