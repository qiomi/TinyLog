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

- Autotools (autoconf, automake, libtool)
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)

### Build Steps

```bash
# Bootstrap the build system
./bootstrap.sh

# Configure the build
./configure

# Build the library and tests
make

# Run tests
make check

# Install the library (requires root privileges)
sudo make install
```

### Configure Options

```bash
# Specify installation prefix
./configure --prefix=/usr/local

# Build only static library
./configure --disable-shared

# Build only shared library
./configure --disable-static

# Enable debug mode
./configure --enable-debug
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
