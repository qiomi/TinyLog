#include "tinylog/logger.h"
#include "tinylog/internal/sink_interface.h"
#include "tinylog/internal/log_utils.h"
#include <cstdio>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace tinylog {

Logger::Logger(const LogConfig& config)
    : config_(config) {
    InitSinks();
}

Logger::Logger(const std::string& config_file_path)
    : config_file_path_(config_file_path) {
    LoadConfigFromFile(config_file_path);
    InitSinks();
    StartConfigFileMonitor();
}

Logger::Logger(Logger&& other) noexcept
    : config_(std::move(other.config_)),
      config_file_path_(std::move(other.config_file_path_)),
      sinks_(std::move(other.sinks_)),
      is_monitoring_(other.is_monitoring_),
      config_monitor_thread_(std::move(other.config_monitor_thread_)) {
    other.is_monitoring_ = false;
}

Logger& Logger::operator=(Logger&& other) noexcept {
    if (this != &other) {
        config_ = std::move(other.config_);
        config_file_path_ = std::move(other.config_file_path_);
        sinks_ = std::move(other.sinks_);
        is_monitoring_ = other.is_monitoring_;
        config_monitor_thread_ = std::move(other.config_monitor_thread_);
        other.is_monitoring_ = false;
    }
    return *this;
}

Logger::~Logger() {
    StopConfigFileMonitor();
    Flush();
}

void Logger::Log(LogLevel level, const std::string& message, const char* filename, const char* function, int line) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    
    // 检查日志级别是否高于配置的级别
    if (level < config_.GetLogLevel()) {
        return;
    }
    
    // 创建日志事件
    internal::LogEvent event;
    event.message = message;
    event.level = level;
    event.timestamp = time(nullptr);
    event.filename = filename;
    event.function = function;
    event.line = line;
    
    // 向所有sink发送日志
    for (const auto& sink : sinks_) {
        sink->log(event);
    }
}

void Logger::LogDebug(const std::string& message, const char* filename, const char* function, int line) {
    Log(LogLevel::kDebug, message, filename, function, line);
}

void Logger::LogInfo(const std::string& message, const char* filename, const char* function, int line) {
    Log(LogLevel::kInfo, message, filename, function, line);
}

void Logger::LogWarn(const std::string& message, const char* filename, const char* function, int line) {
    Log(LogLevel::kWarn, message, filename, function, line);
}

void Logger::LogError(const std::string& message, const char* filename, const char* function, int line) {
    Log(LogLevel::kError, message, filename, function, line);
}

void Logger::LogFatal(const std::string& message, const char* filename, const char* function, int line) {
    Log(LogLevel::kFatal, message, filename, function, line);
}

void Logger::SetLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    config_.SetLogLevel(level);
}

LogLevel Logger::GetLogLevel() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return config_.GetLogLevel();
}

void Logger::SetLogSink(LogSink sink) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    config_.SetLogSink(sink);
    ReInitSinks();
}

LogSink Logger::GetLogSink() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    return config_.GetLogSink();
}

void Logger::SetConfig(const LogConfig& config) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    config_ = config;
    ValidateConfig();
    ReInitSinks();
}

void Logger::Flush() {
    std::lock_guard<std::mutex> lock(config_mutex_);
    for (const auto& sink : sinks_) {
        sink->flush();
    }
}

void Logger::LoadConfigFromFile(const std::string& config_file_path) {
    std::ifstream file(config_file_path);
    if (!file.is_open()) {
        fprintf(stderr, "Failed to open config file: %s\n", config_file_path.c_str());
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // 跳过空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // 解析键值对
        size_t pos = line.find('=');
        if (pos == std::string::npos) {
            continue;
        }
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        internal::Trim(key);
        internal::Trim(value);
        
        // 设置配置
        if (key == "log_level") {
            config_.SetLogLevel(internal::StringToLogLevel(value));
        } else if (key == "log_sink") {
            config_.SetLogSink(internal::StringToLogSink(value));
        } else if (key == "file_path") {
            config_.SetFilePath(value);
        } else if (key == "max_file_count") {
            try {
                config_.SetMaxFileCount(std::stoi(value));
            } catch (...) {
                // 忽略无效值
            }
        } else if (key == "max_file_size") {
            try {
                config_.SetMaxFileSize(std::stoul(value));
            } catch (...) {
                // 忽略无效值
            }
        } else if (key == "async_mode") {
            config_.SetAsyncMode(value == "true" || value == "1");
        }
    }
    
    file.close();
}

void Logger::InitSinks() {
    sinks_.clear();
    
    // 根据配置创建日志输出目标
    switch (config_.GetLogSink()) {
        case LogSink::kConsole:
            sinks_.emplace_back(std::make_shared<internal::ConsoleSink>());
            break;
        case LogSink::kFile:
            sinks_.emplace_back(std::make_shared<internal::FileSink>(
                config_.GetFilePath(),
                config_.GetMaxFileCount(),
                config_.GetMaxFileSize()));
            break;
        case LogSink::kBoth:
            sinks_.emplace_back(std::make_shared<internal::ConsoleSink>());
            sinks_.emplace_back(std::make_shared<internal::FileSink>(
                config_.GetFilePath(),
                config_.GetMaxFileCount(),
                config_.GetMaxFileSize()));
            break;
        default:
            break;
    }
}

void Logger::ReInitSinks() {
    InitSinks();
}

void Logger::ValidateConfig() {
    if (!config_.Validate()) {
        fprintf(stderr, "Invalid log config, resetting to default\n");
        config_.ResetToDefault();
    }
}

void Logger::StartConfigFileMonitor() {
    if (!config_file_path_.empty() && std::filesystem::exists(config_file_path_)) {
        is_monitoring_ = true;
        config_monitor_thread_ = std::thread(&Logger::MonitorConfigFile, this);
    }
}

void Logger::StopConfigFileMonitor() {
    is_monitoring_ = false;
    if (config_monitor_thread_.joinable()) {
        config_monitor_thread_.join();
    }
}

void Logger::MonitorConfigFile() {
    if (config_file_path_.empty()) {
        return;
    }
    
    time_t last_modified = internal::GetFileLastModifiedTime(config_file_path_);
    
    while (is_monitoring_) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        time_t current_modified = internal::GetFileLastModifiedTime(config_file_path_);
        if (current_modified != last_modified) {
            // 配置文件已修改，重新加载配置
            std::lock_guard<std::mutex> lock(config_mutex_);
            LoadConfigFromFile(config_file_path_);
            ValidateConfig();
            ReInitSinks();
            last_modified = current_modified;
        }
    }
}

}  // namespace tinylog
