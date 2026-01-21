cmake_minimum_required(VERSION 3.16)

project(TinyLog 
    VERSION 1.0.0
    DESCRIPTION "A tiny, lightweight and extensible logging library for C and C++"
    LANGUAGES CXX C
)

# 设置默认编译模式为 Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

# 编译配置
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

# 通用编译选项
add_compile_options(
    -Wall -Wextra -Wpedantic
    -fPIC
    -Wno-unused-parameter
)

# 分模式设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -g)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG -Werror)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-O2 -g -DNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_options(-Os -DNDEBUG)
endif()

# 收集源代码和头文件
file(GLOB_RECURSE TINYLOG_SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cc
    ${PROJECT_SOURCE_DIR}/src/internal/*.cc
)

file(GLOB TINYLOG_PUBLIC_HEADERS
    ${PROJECT_SOURCE_DIR}/include/tinylog/*.h
)

# 构建库
add_library(tinylog ${TINYLOG_SOURCES})

# 设置库属性
set_target_properties(tinylog PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${TINYLOG_PUBLIC_HEADERS}"
)

# 设置库的包含目录
target_include_directories(tinylog 
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/tinylog/internal
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

# 导出目标配置
install(EXPORT tinylog-targets
    FILE tinylog-config.cmake
    NAMESPACE TinyLog::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tinylog
)

# 生成pkg-config文件
if(ENABLE_PKG_CONFIG)
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
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
        set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 10)
    endforeach()
endif()

# 打印编译信息
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