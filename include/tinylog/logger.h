#ifndef TINYLOG_LOGGER_H_
#define TINYLOG_LOGGER_H_

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "log_config.h"
#include "log_manager.h"

namespace tinylog {

namespace internal {
class SinkInterface;
}

// 日志类，用于记录日志
class Logger {
public:
    explicit Logger(const LogConfig& config = LogConfig());
    explicit Logger(const std::string& config_file_path);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    Logger(Logger&&) noexcept;
    Logger& operator=(Logger&&) noexcept;

    ~Logger();

    // 核心日志记录函数
    void Log(LogLevel level, const std::string& message, const char* filename, const char* function, int line);

    // 便捷日志记录函数
    void LogDebug(const std::string& message, const char* filename, const char* function, int line);
    void LogInfo(const std::string& message, const char* filename, const char* function, int line);
    void LogWarn(const std::string& message, const char* filename, const char* function, int line);
    void LogError(const std::string& message, const char* filename, const char* function, int line);
    void LogFatal(const std::string& message, const char* filename, const char* function, int line);

    // 设置日志级别
    void SetLogLevel(LogLevel level);
    // 获取日志级别
    LogLevel GetLogLevel() const;

    // 设置日志输出目标
    void SetLogSink(LogSink sink);
    // 获取日志输出目标
    LogSink GetLogSink() const;

    // 设置日志配置
    void SetConfig(const LogConfig& config);

    // 刷新日志缓存
    void Flush();

private:
    // 从文件加载配置
    void LoadConfigFromFile(const std::string& config_file_path);
    // 初始化日志输出目标
    void InitSinks();
    // 重新初始化日志输出目标
    void ReInitSinks();
    // 验证配置有效性
    void ValidateConfig();

    // 配置文件监控相关
    void StartConfigFileMonitor();
    void StopConfigFileMonitor();
    void MonitorConfigFile();

    LogConfig config_;
    std::string config_file_path_;

    std::vector<std::shared_ptr<internal::SinkInterface>> sinks_;

    mutable std::mutex config_mutex_;
    bool is_monitoring_ = false;
    std::thread config_monitor_thread_;
};

}  // namespace tinylog

// 宏定义，方便用户调用日志函数，自动传入文件名、函数名和行号
// 全局日志宏，无需显式传入logger实例
#define LOG_DEBUG(message) tinylog::LogManager::GetInstance().GetGlobalLogger().LogDebug(message, __FILE__, __func__, __LINE__)
#define LOG_INFO(message) tinylog::LogManager::GetInstance().GetGlobalLogger().LogInfo(message, __FILE__, __func__, __LINE__)
#define LOG_WARN(message) tinylog::LogManager::GetInstance().GetGlobalLogger().LogWarn(message, __FILE__, __func__, __LINE__)
#define LOG_ERROR(message) tinylog::LogManager::GetInstance().GetGlobalLogger().LogError(message, __FILE__, __func__, __LINE__)
#define LOG_FATAL(message) tinylog::LogManager::GetInstance().GetGlobalLogger().LogFatal(message, __FILE__, __func__, __LINE__)

// 模块日志宏，需要指定模块名
#define LOG_MODULE_DEBUG(module_name, message) tinylog::LogManager::GetInstance().GetModuleLogger(module_name).LogDebug(message, __FILE__, __func__, __LINE__)
#define LOG_MODULE_INFO(module_name, message) tinylog::LogManager::GetInstance().GetModuleLogger(module_name).LogInfo(message, __FILE__, __func__, __LINE__)
#define LOG_MODULE_WARN(module_name, message) tinylog::LogManager::GetInstance().GetModuleLogger(module_name).LogWarn(message, __FILE__, __func__, __LINE__)
#define LOG_MODULE_ERROR(module_name, message) tinylog::LogManager::GetInstance().GetModuleLogger(module_name).LogError(message, __FILE__, __func__, __LINE__)
#define LOG_MODULE_FATAL(module_name, message) tinylog::LogManager::GetInstance().GetModuleLogger(module_name).LogFatal(message, __FILE__, __func__, __LINE__)

#endif  // TINYLOG_LOGGER_H_
