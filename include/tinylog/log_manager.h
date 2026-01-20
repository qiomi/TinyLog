#ifndef TINYLOG_LOG_MANAGER_H_
#define TINYLOG_LOG_MANAGER_H_

#include <memory>
#include <string>
#include "log_config.h"

namespace tinylog {

class Logger;

// 日志管理器，用于管理全局日志和模块日志
class LogManager {
public:
    // 获取日志管理器单例
    static LogManager& GetInstance();
    
    // 禁止复制和移动
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;
    LogManager(LogManager&&) noexcept = delete;
    LogManager& operator=(LogManager&&) noexcept = delete;
    
    ~LogManager();
    
    // 初始化全局日志配置
    void InitGlobalLogger(const LogConfig& config);
    void InitGlobalLogger(const std::string& config_file_path);
    
    // 获取全局日志实例
    Logger& GetGlobalLogger();
    
    // 获取或创建模块日志实例
    Logger& GetModuleLogger(const std::string& module_name);
    Logger& GetModuleLogger(const std::string& module_name, const LogConfig& config);
    
    // 设置全局日志级别
    void SetGlobalLogLevel(LogLevel level);
    
    // 设置模块日志级别
    void SetModuleLogLevel(const std::string& module_name, LogLevel level);
    
    // 刷新所有日志实例
    void FlushAll();
    
private:
    LogManager();
    
    // 全局日志实例
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace tinylog

#endif  // TINYLOG_LOG_MANAGER_H_
