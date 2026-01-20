#include "file_sink.h"

#include <cstdlib>
#include <iostream>

namespace tinylog::internal {

FileSink::FileSink(const std::string& filepath) : filepath_(filepath) {
    file_ptr_ = fopen(filepath_.c_str(), "ab+");
    if (file_ptr_ == nullptr) {
        std::cerr << "Failed to open log file: " << filepath_ << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

FileSink::~FileSink() {
    if (file_ptr_ != nullptr) {
        flush();
        fclose(file_ptr_);
        file_ptr_ = nullptr;
    }
}

void FileSink::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (file_ptr_ == nullptr) {
        return;
    }

    fwrite(message.c_str(), 1, message.size(), file_ptr_);
}

void FileSink::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (file_ptr_ != nullptr) {
        fflush(file_ptr_);
    }
}
}  // namespace tinylog::internal