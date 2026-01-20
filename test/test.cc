#include <iostream>

#include "tinylog/logger.h"

int main() {
    std::cout << "Running TinyLog tests..." << std::endl;

    // Test LogConfig class
    std::cout << "Testing LogConfig class..." << std::endl;

    // Test default constructor
    tinylog::LogConfig config1;
    if (config1.GetLogLevel() == tinylog::LogLevel::kInfo && config1.GetLogSink() == tinylog::LogSink::kConsole &&
        config1.GetFilePath().empty() && config1.GetMaxFileCount() == 5 && config1.GetMaxFileSize() == 1024 * 1024 &&
        !config1.IsAsyncMode()) {
        std::cout << "✓ Default constructor test passed" << std::endl;
    } else {
        std::cout << "✗ Default constructor test failed" << std::endl;
    }

    // Test parameterized constructor
    tinylog::LogConfig config2(tinylog::LogLevel::kDebug, tinylog::LogSink::kFile, "test.log", 10, 2 * 1024 * 1024,
                               true);
    if (config2.GetLogLevel() == tinylog::LogLevel::kDebug && config2.GetLogSink() == tinylog::LogSink::kFile &&
        config2.GetFilePath() == "test.log" && config2.GetMaxFileCount() == 10 &&
        config2.GetMaxFileSize() == 2 * 1024 * 1024 && config2.IsAsyncMode()) {
        std::cout << "✓ Parameterized constructor test passed" << std::endl;
    } else {
        std::cout << "✗ Parameterized constructor test failed" << std::endl;
    }

    // Test setter and getter methods
    config1.SetLogLevel(tinylog::LogLevel::kError);
    config1.SetLogSink(tinylog::LogSink::kBoth);
    config1.SetFilePath("app.log");
    config1.SetMaxFileCount(3);
    config1.SetMaxFileSize(5 * 1024 * 1024);
    config1.SetAsyncMode(true);

    if (config1.GetLogLevel() == tinylog::LogLevel::kError && config1.GetLogSink() == tinylog::LogSink::kBoth &&
        config1.GetFilePath() == "app.log" && config1.GetMaxFileCount() == 3 &&
        config1.GetMaxFileSize() == 5 * 1024 * 1024 && config1.IsAsyncMode()) {
        std::cout << "✓ Setter and Getter methods test passed" << std::endl;
    } else {
        std::cout << "✗ Setter and Getter methods test failed" << std::endl;
    }

    // Test ResetToDefault method
    config1.ResetToDefault();
    if (config1.GetLogLevel() == tinylog::LogLevel::kInfo && config1.GetLogSink() == tinylog::LogSink::kConsole &&
        config1.GetFilePath().empty() && config1.GetMaxFileCount() == 5 && config1.GetMaxFileSize() == 1024 * 1024 &&
        !config1.IsAsyncMode()) {
        std::cout << "✓ ResetToDefault method test passed" << std::endl;
    } else {
        std::cout << "✗ ResetToDefault method test failed" << std::endl;
    }

    // Test Validate method
    // Valid config
    tinylog::LogConfig valid_config(tinylog::LogLevel::kInfo, tinylog::LogSink::kFile, "valid.log", 5, 10 * 1024 * 1024,
                                    false);
    if (valid_config.Validate()) {
        std::cout << "✓ Valid config validation test passed" << std::endl;
    } else {
        std::cout << "✗ Valid config validation test failed" << std::endl;
    }

    // Invalid config: file sink with empty path
    tinylog::LogConfig invalid_config1(tinylog::LogLevel::kInfo, tinylog::LogSink::kFile, "", 5, 10 * 1024 * 1024,
                                       false);
    if (!invalid_config1.Validate()) {
        std::cout << "✓ Invalid config (empty path) validation test passed" << std::endl;
    } else {
        std::cout << "✗ Invalid config (empty path) validation test failed" << std::endl;
    }

    // Invalid config: invalid file count
    tinylog::LogConfig invalid_config2(tinylog::LogLevel::kInfo, tinylog::LogSink::kConsole, "", 0, 10 * 1024 * 1024,
                                       false);
    if (!invalid_config2.Validate()) {
        std::cout << "✓ Invalid config (invalid file count) validation test passed" << std::endl;
    } else {
        std::cout << "✗ Invalid config (invalid file count) validation test failed" << std::endl;
    }

    // Invalid config: invalid file size
    tinylog::LogConfig invalid_config3(tinylog::LogLevel::kInfo, tinylog::LogSink::kConsole, "", 5, 1023, false);
    if (!invalid_config3.Validate()) {
        std::cout << "✓ Invalid config (invalid file size) validation test passed" << std::endl;
    } else {
        std::cout << "✗ Invalid config (invalid file size) validation test failed" << std::endl;
    }

    std::cout << "All tests completed!" << std::endl;

    return 0;
}