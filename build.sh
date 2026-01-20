#!/bin/bash

# ===================== 配置项 =====================
DEFAULT_BUILD_TYPE="Debug"  # 默认编译模式
DEFAULT_INSTALL_PREFIX="/usr/local"  # 默认安装前缀
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

# 显示使用帮助
show_help() {
    echo "Usage: $0 [OPTIONS] [debug|release]"
    echo ""
    echo "Options:"
    echo "  -h, --help        Show this help message and exit"
    echo "  -i, --install     Install the library after building"
    echo "  -p, --prefix DIR  Specify installation prefix (default: $DEFAULT_INSTALL_PREFIX)"
    echo "  -s, --sudo        Use sudo for installation"
    echo ""
    echo "Examples:"
    echo "  $0                  # Build in debug mode"
    echo "  $0 release          # Build in release mode"
    echo "  $0 -i              # Build and install in debug mode"
    echo "  $0 -i -s release    # Build in release mode and install with sudo"
    echo "  $0 -i -p /opt/tinylog # Build and install to custom prefix"
    exit 0
}

# ===================== 解析参数 =====================
BUILD_TYPE="$DEFAULT_BUILD_TYPE"
INSTALL=false
USE_SUDO=false
INSTALL_PREFIX="$DEFAULT_INSTALL_PREFIX"

# 解析长选项
while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)
            show_help
            ;;
        -i|--install)
            INSTALL=true
            shift
            ;;
        -s|--sudo)
            USE_SUDO=true
            shift
            ;;
        -p|--prefix)
            if [ -n "$2" ] && [[ "$2" != -* ]]; then
                INSTALL_PREFIX="$2"
                shift 2
            else
                print_error "Missing argument for --prefix"
            fi
            ;;
        --prefix=*)
            INSTALL_PREFIX="${1#*=}"
            shift
            ;;
        *)
            # 处理构建类型参数
            if [ -n "$1" ]; then
                BUILD_TYPE=$(echo "$1" | tr '[:upper:]' '[:lower:]')
                if [ "$BUILD_TYPE" != "debug" ] && [ "$BUILD_TYPE" != "release" ]; then
                    print_error "Invalid build type! Usage: $0 [debug|release]"
                fi
                shift
            fi
            ;;
    esac
done

# ===================== 清理旧构建 =====================
print_info "Cleaning old build files..."
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR" || print_error "Failed to create build directory!"

# ===================== 执行CMake =====================
print_info "Running CMake (Build Type: $BUILD_TYPE)..."
cd "$BUILD_DIR" || print_error "Failed to enter build directory!"

# 指定编译模式和安装前缀执行cmake
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" ..
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
# 检查库文件（静态或动态）
if [ "$BUILD_TYPE" = "debug" ]; then
    LIB_FILE="./lib/libtinylog.a"
    SO_FILE="./lib/libtinylog.so"
else
    LIB_FILE="./lib/libtinylog.a"
    SO_FILE="./lib/libtinylog.so"
fi

if [ -f "$LIB_FILE" ]; then
    print_success "Static library built successfully: $LIB_FILE"
elif [ -f "$SO_FILE" ]; then
    print_success "Shared library built successfully: $SO_FILE"
else
    print_error "No library file found!"
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

# ===================== 安装项目 =====================
if [ "$INSTALL" = true ]; then
    print_info "Installing TinyLog to $INSTALL_PREFIX..."
    
    # 执行安装命令，根据需要使用sudo
    if [ "$USE_SUDO" = true ]; then
        sudo make install
    else
        make install
    fi
    
    if [ $? -ne 0 ]; then
        print_error "Installation failed!"
    else
        print_success "✨ Installation completed successfully!"
        print_info "Installed files:"
        echo -e "   - Library: $INSTALL_PREFIX/lib/"
        echo -e "   - Headers: $INSTALL_PREFIX/include/tinylog/"
        echo -e "   - CMake config: $INSTALL_PREFIX/lib/cmake/tinylog/"
        if [ -f "$INSTALL_PREFIX/lib/pkgconfig/tinylog.pc" ]; then
            echo -e "   - Pkg-config: $INSTALL_PREFIX/lib/pkgconfig/tinylog.pc"
        fi
    fi
fi

# ===================== 完成提示 =====================
cd ..
print_success "✨ Build completed successfully! (Mode: $BUILD_TYPE)"
echo -e "${COLOR_BLUE}📁 Output files:${COLOR_NC}"
echo -e "   - Library: $BUILD_DIR/lib/"
echo -e "   - Test binaries:  $BUILD_DIR/bin/"
echo -e "   - Compile commands: $BUILD_DIR/compile_commands.json"
echo -e "${COLOR_BLUE}💡 Tips:${COLOR_NC}"
echo -e "   - Debug mode: warnings are shown, no error termination"
echo -e "   - Release mode: warnings are treated as errors"
echo -e "   - Run tests: cd $BUILD_DIR && ctest"
echo -e "   - Install: $0 -i [debug|release]"
if [ "$INSTALL" = false ]; then
    echo -e "   - Quick install: $0 -i"
fi