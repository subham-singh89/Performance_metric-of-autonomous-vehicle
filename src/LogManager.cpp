#include "LogManager.h"

LogManager::LogManager(const std::string& config_file)
    : running(true), current_file_size(0), log_file_index(0) {
    
    try {
        std::ifstream file(config_file);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open config file: " + config_file);
        }
        
        json config;
        file >> config;

        log_file_name = config["log_file"];
        std::string log_level_str = config["log_level"];
        max_file_size = config["max_file_size"];

        if (log_level_str == "INFO") {
            configured_level = LogLevel::INFO;
        } else if (log_level_str == "DEBUG") {
            configured_level = LogLevel::DEBUG;
        } else if (log_level_str == "ERROR") {
            configured_level = LogLevel::ERROR;
        } else if (log_level_str == "PERFORMANCE") {
            configured_level = LogLevel::PERFORMANCE;
        } else {
            throw std::runtime_error("Invalid log level in config file");
        }

        logging_thread = std::thread(&LogManager::process_logs, this);
    } catch (const std::exception& e) {
        std::cerr << "Error in LogManager constructor: " << e.what() << std::endl;
        // Optionally, handle or log the error further as needed
        throw; // Re-throw the exception to propagate it further if desired
    }
}


LogManager::~LogManager() {
    stop();
}

void LogManager::log(LogLevel level, const std::string& message) {
    if (level >= configured_level) {
        for (auto& log_msg : log_messages) {
            if (log_queue.push(&log_msg)) {
                log_msg.level = level;
                strncpy(log_msg.message, message.c_str(), sizeof(log_msg.message) - 1);
                log_msg.message[sizeof(log_msg.message) - 1] = '\0';
                break;
            }
        }
    }
}

void LogManager::logPerformance(const std::string& metric_name, double value) {
    std::string message = metric_name + ": " + std::to_string(value) + " ms";
    log(LogLevel::PERFORMANCE, message);
}

void LogManager::stop() {
    running = false;
    logging_thread.join();
    if (log_file.is_open()) {
        log_file.close();
    }
}

void LogManager::process_logs() {
    log_file.open(get_new_log_file_name(), std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        throw std::runtime_error("Unable to open log file");
    }

    while (running) {
        LogMessage* log_entry;
        while (log_queue.pop(log_entry)) {
            std::string log_message = get_timestamp() + " " + log_level_to_string(log_entry->level) + ": " + log_entry->message + "\n";
            log_file << log_message;
            current_file_size += log_message.size();
            check_and_rotate_log();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    LogMessage* log_entry;
    while (log_queue.pop(log_entry)) {
        std::string log_message = get_timestamp() + " " + log_level_to_string(log_entry->level) + ": " + log_entry->message + "\n";
        log_file << log_message;
    }
}

void LogManager::check_and_rotate_log() {
    if (current_file_size >= max_file_size) {
        log_file.close();
        log_file.open(get_new_log_file_name(), std::ios::out | std::ios::app);
        if (!log_file.is_open()) {
            throw std::runtime_error("Unable to open log file");
        }
        current_file_size = 0;
    }
}

std::string LogManager::log_level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::PERFORMANCE: return "PERFORMANCE";
        default: return "UNKNOWN";
    }
}

std::string LogManager::get_new_log_file_name() {
    return log_file_name + "_" + std::to_string(++log_file_index) + ".log";
}

std::string LogManager::get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}
