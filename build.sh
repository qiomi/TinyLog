#!/bin/bash

# ===================== 配置项 =====================
DEFAULT_BUILD_TYPE="Debug"  # 默认编译模式
BUILD_DIR="./build"
COLOR_RED='\033[0;31m'
COLOR_GREEN='\033[0;32m'
COLOR_YELLOW='\033[1;33m'
COLOR_BLUE='\033[0;34m'
COLOR_NC='\033[0m' # 重置颜色

# ===================== 函数定义 =====================
# 打印带颜色的信息
print_info() {
    echo -e "${COLOR_BLUE}[INFO] $1${COLOR_NC}"
}

print_warn() {
    echo -e "${COLOR_YELLOW}[WARN] $1${COLOR_NC}"
}

print_success() {
    echo -e "${COLOR_GREEN}[SUCCESS] $1${COLOR_NC}"
}

print_error() {
    echo -e "${COLOR_RED}[ERROR] $1${COLOR_NC}"
    exit 1
}

# ===================== 解析参数 =====================
# 支持参数：debug/release（不区分大小写）
if [ $# -eq 1 ]; then
    BUILD_TYPE=$(echo "$1" | tr '[:upper:]' '[:lower:]')
    if [ "$BUILD_TYPE" != "debug" ] && [ "$BUILD_TYPE" != "release" ]; then
        print_error "Invalid parameter! Usage: $0 [debug|release] (default: debug)"
    fi
else
    BUILD_TYPE="$DEFAULT_BUILD_TYPE"
fi

# ===================== 清理旧构建 =====================
print_info "Cleaning old build files..."
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR" || print_error "Failed to create build directory!"

# ===================== 执行CMake =====================
print_info "Running CMake (Build Type: $BUILD_TYPE)..."
cd "$BUILD_DIR" || print_error "Failed to enter build directory!"

# 指定编译模式执行cmake
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..
if [ $? -ne 0 ]; then
    print_error "CMake configuration failed!"
fi

# ===================== 编译项目 =====================
print_info "Compiling TinyLog (Build Type: $BUILD_TYPE)..."
make -j$(nproc)
if [ $? -ne 0 ]; then
    print_error "Compilation failed!"
fi

# ===================== 验证输出 =====================
# 检查静态库
LIB_FILE="./lib/libtinylog.a"
if [ -f "$LIB_FILE" ]; then
    print_success "Static library built successfully: $LIB_FILE"
else
    print_error "Static library not found!"
fi

# 检查测试程序
TEST_BIN_DIR="./bin"
if [ -d "$TEST_BIN_DIR" ] && [ "$(ls -A $TEST_BIN_DIR)" ]; then
    print_success "Test executables built successfully: $TEST_BIN_DIR"
    print_info "Test files list:"
    ls -lh "$TEST_BIN_DIR"
else
    print_warn "No test executables found (check test/ directory)"
fi

# ===================== 完成提示 =====================
cd ..
print_success "✨ Build completed successfully! (Mode: $BUILD_TYPE)"
echo -e "${COLOR_BLUE}📁 Output files:${COLOR_NC}"
echo -e "   - Static library: $BUILD_DIR/lib/libtinylog.a"
echo -e "   - Test binaries:  $BUILD_DIR/bin/"
echo -e "   - Compile commands: $BUILD_DIR/compile_commands.json"
echo -e "${COLOR_BLUE}💡 Tips:${COLOR_NC}"
echo -e "   - Debug mode: warnings are shown, no error termination"
echo -e "   - Release mode: warnings are treated as errors"
echo -e "   - Switch mode: ./build.sh release"