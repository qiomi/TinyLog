# TinyLog

A tiny, lightweight and extensible logging library for C++17.

## Features

- Simple and easy to use API
- Global and module-level logging support
- Multiple log levels (Debug, Info, Warn, Error, Fatal)
- Support for both console and file output
- File rotation support
- Thread-safe design
- Configurable via file
- C++17 support
- Both static and dynamic library support

## Build and Install

### Prerequisites

- CMake 3.16+
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)

### Build Steps

```bash
# Create build directory
mkdir -p build && cd build

# Configure the build
cmake ..

# Build the library and tests
make -j$(nproc)

# Run tests
ctest

# Install the library (requires root privileges)
sudo make install
```

### Build Options

```bash
# Specify installation prefix
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local

# Build Release mode
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build shared library instead of static library
cmake .. -DBUILD_SHARED_LIBS=ON

# Disable tests
cmake .. -DBUILD_TESTING=OFF

# Disable pkg-config file generation
cmake .. -DENABLE_PKG_CONFIG=OFF
```

### Multi-Configuration Build Systems (e.g., Visual Studio)

```bash
# Create build directory
mkdir -p build && cd build

# Configure the build
cmake .. -G "Visual Studio 16 2019" -A x64

# Build the library (Release mode)
cmake --build . --config Release

# Install the library
cmake --install . --config Release
```

## Usage

### Basic Usage

```cpp
#include "tinylog/logger.h"

int main() {
    // Global logging
    LOG_INFO("This is an info message");
    LOG_WARN("This is a warning message");
    LOG_ERROR("This is an error message");
    
    // Module logging
    LOG_MODULE_INFO("module1", "This is a module info message");
    LOG_MODULE_WARN("module2", "This is a module warning message");
    
    return 0;
}
```

### Linking with TinyLog

```bash
g++ -o myapp myapp.cpp -ltinylog
```

Using pkg-config:

```bash
g++ -o myapp myapp.cpp $(pkg-config --cflags --libs tinylog)
```

## Configuration

### Log Levels

- `LogLevel::kDebug`
- `LogLevel::kInfo`
- `LogLevel::kWarn`
- `LogLevel::kError`
- `LogLevel::kFatal`

### Log Sinks

- `LogSink::kConsole` - Output to console
- `LogSink::kFile` - Output to file
- `LogSink::kBoth` - Output to both console and file

## License

MIT License
