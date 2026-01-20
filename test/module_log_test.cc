#include <iostream>
#include "tinylog/logger.h"

using namespace tinylog;

int main() {
    std::cout << "Testing global and module logging features...\n";
    
    // 测试1: 使用全局日志宏，无需显式创建logger实例
    std::cout << "\n1. Testing global logging macros...\n";
    LOG_DEBUG("This is a global debug message");
    LOG_INFO("This is a global info message");
    LOG_WARN("This is a global warning message");
    LOG_ERROR("This is a global error message");
    LOG_FATAL("This is a global fatal message");
    
    // 测试2: 使用模块日志宏
    std::cout << "\n2. Testing module logging macros...\n";
    LOG_MODULE_DEBUG("module1", "This is a debug message from module1");
    LOG_MODULE_INFO("module1", "This is an info message from module1");
    LOG_MODULE_WARN("module2", "This is a warning message from module2");
    LOG_MODULE_ERROR("module2", "This is an error message from module2");
    
    // 测试3: 设置不同模块的日志级别
    std::cout << "\n3. Testing module log level setting...\n";
    LogManager::GetInstance().SetModuleLogLevel("module1", LogLevel::kError);
    LogManager::GetInstance().SetModuleLogLevel("module2", LogLevel::kDebug);
    
    // 测试4: 验证模块日志级别设置是否生效
    std::cout << "\n4. Verifying module log level settings...\n";
    LOG_MODULE_DEBUG("module1", "This debug message from module1 should NOT appear");
    LOG_MODULE_ERROR("module1", "This error message from module1 should appear");
    LOG_MODULE_DEBUG("module2", "This debug message from module2 should appear");
    
    // 测试5: 测试全局日志级别设置
    std::cout << "\n5. Testing global log level setting...\n";
    LogManager::GetInstance().SetGlobalLogLevel(LogLevel::kWarn);
    LOG_DEBUG("This global debug message should NOT appear");
    LOG_WARN("This global warning message should appear");
    
    // 测试6: 刷新所有日志
    std::cout << "\n6. Flushing all logs...\n";
    LogManager::GetInstance().FlushAll();
    
    std::cout << "\nAll tests completed!\n";
    return 0;
}