# TinyLog

一个小巧、轻量级且可扩展的C++17日志库。

## 特性

- 简单易用的API
- 支持全局和模块级别的日志
- 多种日志级别（Debug, Info, Warn, Error, Fatal）
- 支持控制台和文件输出
- 文件滚动支持
- 线程安全设计
- 支持通过文件配置
- C++17支持
- 同时支持静态库和动态库

## 构建与安装

### 前提条件

- CMake 3.16+
- 兼容C++17的编译器（GCC 7+, Clang 5+, MSVC 2017+）

### 构建步骤

```bash
# 创建构建目录
mkdir -p build && cd build

# 配置构建
cmake ..

# 构建库和测试
make -j$(nproc)

# 运行测试
ctest

# 安装库（需要root权限）
sudo make install
```

### 构建选项

```bash
# 指定安装前缀
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local

# 构建Release模式
cmake .. -DCMAKE_BUILD_TYPE=Release

# 构建动态库而非静态库
cmake .. -DBUILD_SHARED_LIBS=ON

# 禁用测试
cmake .. -DBUILD_TESTING=OFF

# 禁用pkg-config文件生成
cmake .. -DENABLE_PKG_CONFIG=OFF
```

### 多配置构建系统（例如Visual Studio）

```bash
# 创建构建目录
mkdir -p build && cd build

# 配置构建
cmake .. -G "Visual Studio 16 2019" -A x64

# 构建库（Release模式）
cmake --build . --config Release

# 安装库
cmake --install . --config Release
```

## 使用

### 基本使用

```cpp
#include "tinylog/logger.h"

int main() {
    // 全局日志
    LOG_INFO("这是一条信息日志");
    LOG_WARN("这是一条警告日志");
    LOG_ERROR("这是一条错误日志");
    
    // 模块日志
    LOG_MODULE_INFO("module1", "这是来自module1的信息日志");
    LOG_MODULE_WARN("module2", "这是来自module2的警告日志");
    
    return 0;
}
```

### 与TinyLog链接

```bash
g++ -o myapp myapp.cpp -ltinylog
```

使用pkg-config：

```bash
g++ -o myapp myapp.cpp $(pkg-config --cflags --libs tinylog)
```

## 配置

### 日志级别

- `LogLevel::kDebug`
- `LogLevel::kInfo`
- `LogLevel::kWarn`
- `LogLevel::kError`
- `LogLevel::kFatal`

### 日志输出目标

- `LogSink::kConsole` - 输出到控制台
- `LogSink::kFile` - 输出到文件
- `LogSink::kBoth` - 同时输出到控制台和文件

## 许可证

MIT License
