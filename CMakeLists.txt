cmake_minimum_required(VERSION 3.16)

project(TinyLog 
    VERSION 1.0.0
    DESCRIPTION "A tiny, lightweight and extensible logging library for C++"
    LANGUAGES CXX
)

# 设置默认编译模式为 Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (Debug/Release)" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 安装目录配置 - 符合GNU标准
include(GNUInstallDirs)

# 选项配置
option(BUILD_SHARED_LIBS "Build shared library instead of static library" OFF)
option(BUILD_TESTING "Build tests" ON)
option(ENABLE_PKG_CONFIG "Generate pkg-config file" ON)

# 通用编译选项（所有模式都开启基础警告）
add_compile_options(
    -Wall -Wextra -Wpedantic
    -fPIC
    -Wno-unused-parameter  # 全局忽略未使用参数的警告
)

# 分模式设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug模式：仅报警告，不转错误；开启调试信息，关闭优化
    add_compile_options(-O0 -g -ggdb)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Release模式：严格检查，警告转错误；开启优化，关闭调试
    add_compile_options(-O3 -DNDEBUG -Werror)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB_RECURSE TINYLOG_SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cc
    ${PROJECT_SOURCE_DIR}/src/internal/*.cc
)

# 只收集公开头文件，排除internal目录下的内部头文件
file(GLOB_RECURSE TINYLOG_HEADERS
    ${PROJECT_SOURCE_DIR}/include/tinylog/*.h
)
file(GLOB_RECURSE TINYLOG_INTERNAL_HEADERS
    ${PROJECT_SOURCE_DIR}/include/tinylog/internal/*.h
)

# 从公开头文件列表中排除内部头文件
foreach(INTERNAL_HEADER ${TINYLOG_INTERNAL_HEADERS})
    list(REMOVE_ITEM TINYLOG_HEADERS "${INTERNAL_HEADER}")
endforeach()

# 构建库
add_library(tinylog ${TINYLOG_SOURCES})

# 设置库属性
set_target_properties(tinylog PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${TINYLOG_HEADERS}"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}
)

# 安装配置
install(TARGETS tinylog
    EXPORT tinylog-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tinylog
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 安装内部头文件到单独目录（可选，默认不安装）
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/tinylog/internal/
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tinylog/internal
#     FILES_MATCHING PATTERN "*.h"
# )

# 导出目标配置
install(EXPORT tinylog-targets
    FILE tinylog-config.cmake
    NAMESPACE TinyLog::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinylog
)

# 生成pkg-config文件
if(ENABLE_PKG_CONFIG AND BUILD_SHARED_LIBS)
    configure_file(
        ${CMAKE_SOURCE_DIR}/tinylog.pc.in
        ${CMAKE_BINARY_DIR}/tinylog.pc
        @ONLY
    )
    install(FILES ${CMAKE_BINARY_DIR}/tinylog.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
endif()

# 构建测试
if(BUILD_TESTING)
    enable_testing()
    file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cc)
    foreach(TEST_FILE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
        # 避免使用"test"作为目标名称，因为它在CTest中是保留关键字
        if(TEST_NAME STREQUAL "test")
            set(TEST_NAME "tinylog_test")
        endif()
        add_executable(${TEST_NAME} ${TEST_FILE})
        target_link_libraries(${TEST_NAME} tinylog)
        set_target_properties(${TEST_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}
        )
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()
endif()

# 打印编译模式提示（便于确认当前模式）
message(STATUS "=====================================")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# 计算库类型
if(BUILD_SHARED_LIBS)
    message(STATUS "Library Type: Shared")
else()
    message(STATUS "Library Type: Static")
endif()

# 计算是否构建测试
if(BUILD_TESTING)
    message(STATUS "Build Tests: Yes")
else()
    message(STATUS "Build Tests: No")
endif()

# 计算是否启用pkg-config
if(ENABLE_PKG_CONFIG)
    message(STATUS "Enable PkgConfig: Yes")
else()
    message(STATUS "Enable PkgConfig: No")
endif()

message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "=====================================")