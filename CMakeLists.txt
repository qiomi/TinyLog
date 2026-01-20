cmake_minimum_required(VERSION 3.16)

project(TinyLog 
    VERSION 1.0.0
    DESCRIPTION "A tiny, lightweight and extensible logging library for C++"
    LANGUAGES CXX
)

# 设置默认编译模式为 Debug（核心）
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (Debug/Release)" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 通用编译选项（所有模式都开启基础警告）
add_compile_options(
    -Wall -Wextra -Wpedantic
    -fPIC
    -Wno-unused-parameter  # 全局忽略未使用参数的警告（可选，也可仅在Debug关闭）
)

# 分模式设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug模式：仅报警告，不转错误；开启调试信息，关闭优化
    add_compile_options(-O0 -g -ggdb)
    # Debug模式关闭-Werror（关键：只报警告，不终止编译）
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Release模式：严格检查，警告转错误；开启优化，关闭调试
    add_compile_options(-O3 -DNDEBUG -Werror)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/internal
    ${PROJECT_SOURCE_DIR}/src
)

file(GLOB_RECURSE TINYLOG_SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cc
    ${PROJECT_SOURCE_DIR}/src/internal/*.cc
    ${PROJECT_SOURCE_DIR}/src/utils.cc
    ${PROJECT_SOURCE_DIR}/src/log_config.cc
    ${PROJECT_SOURCE_DIR}/src/log_manager.cc
    ${PROJECT_SOURCE_DIR}/src/logger.cc
)

# 排除旧的sink目录下的文件，只使用internal目录下的新实现
list(FILTER TINYLOG_SOURCES EXCLUDE REGEX ".*/src/sink/.*\.cc$")

file(GLOB_RECURSE TINYLOG_HEADERS
    ${PROJECT_SOURCE_DIR}/include/*.h
)

add_library(tinylog STATIC ${TINYLOG_SOURCES} ${TINYLOG_HEADERS})

set_target_properties(tinylog PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/tinylog
    FILES_MATCHING PATTERN "*.h"
)

install(TARGETS tinylog
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cc)
foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} tinylog)
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    )
endforeach()

# 打印编译模式提示（便于确认当前模式）
message(STATUS "=====================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Mode: Debug (warnings enabled, no error)")
else()
    message(STATUS "Mode: Release (warnings as errors)")
endif()
message(STATUS "=====================================")