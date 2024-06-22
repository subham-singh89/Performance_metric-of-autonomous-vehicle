#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <string>
#include <boost/lockfree/queue.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <cstring>

using json = nlohmann::json;

enum class LogLevel {
    INFO,
    DEBUG,
    ERROR,
    PERFORMANCE
};

class LogManager {
public:
    LogManager(const std::string& config_file);
    ~LogManager();

    void log(LogLevel level, const std::string& message);
    void logPerformance(const std::string& metric_name, double value);
    void stop();

private:
    struct LogMessage {
        LogLevel level;
        char message[256];
    };

    void process_logs();
    void check_and_rotate_log();
    std::string log_level_to_string(LogLevel level);
    std::string get_new_log_file_name();
    std::string get_timestamp();

    std::atomic<bool> running;
    boost::lockfree::queue<LogMessage*, boost::lockfree::capacity<128>> log_queue;
    std::thread logging_thread;
    std::ofstream log_file;
    std::string log_file_name;
    LogLevel configured_level;
    size_t max_file_size;
    size_t current_file_size;
    size_t log_file_index;
    LogMessage log_messages[128];
};

#endif // LOGMANAGER_H
