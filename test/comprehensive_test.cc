#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#include "tinylog/logger.h"

void TestBasicLogging(tinylog::Logger& logger) {
    std::cout << "Testing basic logging..." << std::endl;

    logger.LogDebug("This is a debug message", __FILE__, __func__, __LINE__);
    logger.LogInfo("This is an info message", __FILE__, __func__, __LINE__);
    logger.LogWarn("This is a warning message", __FILE__, __func__, __LINE__);
    logger.LogError("This is an error message", __FILE__, __func__, __LINE__);
    logger.LogFatal("This is a fatal message", __FILE__, __func__, __LINE__);

    std::cout << "✓ Basic logging test completed" << std::endl;
}

void TestFileLogging() {
    std::cout << "Testing file logging..." << std::endl;

    // 创建文件日志配置
    tinylog::LogConfig config;
    config.SetLogLevel(tinylog::LogLevel::kDebug);
    config.SetLogSink(tinylog::LogSink::kFile);
    config.SetFilePath("test_log_file.log");
    config.SetMaxFileCount(3);
    config.SetMaxFileSize(1024 * 100);  // 100KB
    config.SetAsyncMode(false);

    tinylog::Logger logger(config);

    // 写入大量日志，测试文件切片功能
    for (int i = 0; i < 2000; ++i) {
        logger.LogInfo(
            "Test log message " + std::to_string(i) + ": This is a test message to check file rotation functionality.",
            __FILE__, __func__, __LINE__);
    }

    logger.Flush();
    std::cout << "✓ File logging test completed" << std::endl;
}

void TestConfigFile() {
    std::cout << "Testing config file..." << std::endl;

    // 创建临时配置文件
    std::ofstream config_file("test_config.ini");
    config_file << "# Test log configuration\n";
    config_file << "log_level=debug\n";
    config_file << "log_sink=both\n";
    config_file << "file_path=test_config.log\n";
    config_file << "max_file_count=2\n";
    config_file << "max_file_size=50000\n";
    config_file << "async_mode=false\n";
    config_file.close();

    // 使用配置文件创建日志器
    tinylog::Logger logger("test_config.ini");

    // 测试日志输出
    logger.LogInfo("Logging with config file settings", __FILE__, __func__, __LINE__);

    // 模拟配置文件更新
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 更新配置文件
    std::ofstream config_file_update("test_config.ini");
    config_file_update << "# Updated test log configuration\n";
    config_file_update << "log_level=info\n";
    config_file_update << "log_sink=console\n";
    config_file_update << "file_path=test_config_updated.log\n";
    config_file_update << "max_file_count=3\n";
    config_file_update << "max_file_size=100000\n";
    config_file_update << "async_mode=true\n";
    config_file_update.close();

    // 等待配置文件监控线程检测到更新
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // 测试更新后的配置
    logger.LogDebug("This debug message should not appear (level changed to info)", __FILE__, __func__, __LINE__);
    logger.LogInfo("This info message should appear", __FILE__, __func__, __LINE__);

    logger.Flush();
    std::cout << "✓ Config file test completed" << std::endl;
}

void TestDynamicConfigUpdate() {
    std::cout << "Testing dynamic config update..." << std::endl;

    // 创建初始配置
    tinylog::LogConfig config;
    config.SetLogLevel(tinylog::LogLevel::kInfo);
    config.SetLogSink(tinylog::LogSink::kConsole);

    tinylog::Logger logger(config);

    // 测试初始配置
    logger.LogInfo("Initial config: info level, console sink", __FILE__, __func__, __LINE__);
    logger.LogDebug("This debug message should not appear initially", __FILE__, __func__, __LINE__);

    // 动态更新配置
    tinylog::LogConfig new_config;
    new_config.SetLogLevel(tinylog::LogLevel::kDebug);
    new_config.SetLogSink(tinylog::LogSink::kBoth);
    new_config.SetFilePath("dynamic_update.log");
    new_config.SetMaxFileCount(2);
    new_config.SetMaxFileSize(50000);
    new_config.SetAsyncMode(false);

    logger.SetConfig(new_config);

    // 测试更新后的配置
    logger.LogDebug("This debug message should appear after config update", __FILE__, __func__, __LINE__);
    logger.LogInfo("Updated config: debug level, both sinks", __FILE__, __func__, __LINE__);

    logger.Flush();
    std::cout << "✓ Dynamic config update test completed" << std::endl;
}

int main() {
    std::cout << "Running comprehensive TinyLog tests..." << std::endl;

    // 创建默认配置的日志器
    tinylog::Logger logger;

    // 测试基本日志功能
    TestBasicLogging(logger);

    // 测试文件日志和切片功能
    TestFileLogging();

    // 测试动态配置更新
    TestDynamicConfigUpdate();

    // 测试配置文件功能
    TestConfigFile();

    std::cout << "All comprehensive tests completed!" << std::endl;

    return 0;
}