#include "sink_interface.h"

#include <iostream>
#include <sstream>

#include "console_sink.h"
#include "file_sink.h"
#include "tinylog/logger_types.h"
#include "utils.h"

namespace tinylog::internal {

std::shared_ptr<SinkInterface> SinkInterface::CreateSink(LogSink type, const std::string& filepath) {
    switch (type) {
        case LogSink::kConsole:
            return std::make_shared<ConsoleSink>();
        case LogSink::kFile:
            if (filepath.empty()) {
                std::cerr << "FileSink requires non-empty filepath!" << std::endl;
                return nullptr;
            }
            return std::make_shared<FileSink>(filepath);
        default:
            std::cerr << "Unsupported sink type: " << LogSinkToString(type) << std::endl;
            return nullptr;
    }
}

std::string SinkInterface::format(const LogEvent& event) {
    std::ostringstream oss;
    oss << "[" << event.timestamp << "] "
        << "[" << LogLevelToString(event.level) << "] "
        << "[" << event.filename << ":" << event.line << "] " << event.message << "\n";
    return oss.str();
}

void SinkInterface::log(const LogEvent& event) { log(format(event)); }

}  // namespace tinylog::internal