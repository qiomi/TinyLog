#include "tinylog/log_manager.h"

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "tinylog/logger.h"

namespace tinylog {

// LogManager::Impl class definition
class LogManager::Impl {
public:
    Impl() {
        // 初始化全局日志实例
        global_logger_ = std::make_unique<Logger>();
    }

    ~Impl() {
        // 清理所有日志实例
        module_loggers_.clear();
        global_logger_.reset();
    }

    // 全局日志实例
    std::unique_ptr<Logger> global_logger_;

    // 模块日志实例映射
    std::unordered_map<std::string, std::unique_ptr<Logger>> module_loggers_;

    // 互斥锁，用于保护模块日志映射
    std::mutex module_loggers_mutex_;
};

// LogManager implementation
LogManager::LogManager() { impl_ = std::make_unique<Impl>(); }

LogManager::~LogManager() { impl_.reset(); }

LogManager& LogManager::GetInstance() {
    static LogManager instance;
    return instance;
}

void LogManager::InitGlobalLogger(const LogConfig& config) { impl_->global_logger_ = std::make_unique<Logger>(config); }

void LogManager::InitGlobalLogger(const std::string& config_file_path) {
    impl_->global_logger_ = std::make_unique<Logger>(config_file_path);
}

Logger& LogManager::GetGlobalLogger() { return *impl_->global_logger_; }

Logger& LogManager::GetModuleLogger(const std::string& module_name) {
    std::lock_guard<std::mutex> lock(impl_->module_loggers_mutex_);

    // 查找模块日志实例
    auto it = impl_->module_loggers_.find(module_name);
    if (it != impl_->module_loggers_.end()) {
        return *it->second;
    }

    // 如果不存在，创建新的模块日志实例
    auto logger = std::make_unique<Logger>();
    Logger& logger_ref = *logger;
    impl_->module_loggers_[module_name] = std::move(logger);

    return logger_ref;
}

Logger& LogManager::GetModuleLogger(const std::string& module_name, const LogConfig& config) {
    std::lock_guard<std::mutex> lock(impl_->module_loggers_mutex_);

    // 查找模块日志实例
    auto it = impl_->module_loggers_.find(module_name);
    if (it != impl_->module_loggers_.end()) {
        // 如果存在，更新配置
        it->second->SetConfig(config);
        return *it->second;
    }

    // 如果不存在，创建新的模块日志实例
    auto logger = std::make_unique<Logger>(config);
    Logger& logger_ref = *logger;
    impl_->module_loggers_[module_name] = std::move(logger);

    return logger_ref;
}

void LogManager::SetGlobalLogLevel(LogLevel level) {
    impl_->global_logger_->SetLogLevel(level);

    // 同时更新所有模块日志的级别
    std::lock_guard<std::mutex> lock(impl_->module_loggers_mutex_);
    for (auto& pair : impl_->module_loggers_) {
        pair.second->SetLogLevel(level);
    }
}

void LogManager::SetModuleLogLevel(const std::string& module_name, LogLevel level) {
    std::lock_guard<std::mutex> lock(impl_->module_loggers_mutex_);

    auto it = impl_->module_loggers_.find(module_name);
    if (it != impl_->module_loggers_.end()) {
        it->second->SetLogLevel(level);
    }
}

void LogManager::FlushAll() {
    // 刷新全局日志
    impl_->global_logger_->Flush();

    // 刷新所有模块日志
    std::lock_guard<std::mutex> lock(impl_->module_loggers_mutex_);
    for (auto& pair : impl_->module_loggers_) {
        pair.second->Flush();
    }
}

}  // namespace tinylog
