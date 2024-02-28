#pragma once

#include <iostream>
#include <mutex>
#include <memory>
#include <filesystem>
#include <fstream>

template<typename... Args>
std::string format(const std::string& fmt, Args... args) {
    const char* buf;
    sprintf(buf, fmt.c_str(), args...);
    return std::string(buf);
}

class Logger
{
public:
    static void Initialize(const std::filesystem::path & path);
    static void Reset(const std::filesystem::path& path);
    static void Destroy();
    static void Log(const std::string& message) { m_instance->Log_(message); }
    static void Print(const std::string& message) { m_instance->Print_(message); }
    static Logger * Get() { return m_instance.get(); }

    // Function to log with format strings
    template<typename... Args>
    void Log(const std::string& format, Args... args) {
        std::stringstream ss;
        // Format the message
        (ss << ... << args);
        std::string message = ss.str();

        // Log the message
        Log(message);
    }

    // Function to print with format strings
    template<typename... Args>
    void Print(const std::string& format, Args... args) {
        std::stringstream ss;
        // Format the message
        (ss << ... << args);
        std::string message = ss.str();

        // Print the message
        Print(message);
    }

    ~Logger() = default;
private:
    Logger(const std::filesystem::path & path);

    void Log_(const std::string& message);
    void Print_(const std::string& message);

    std::mutex mutex_; // Mutex for thread safety
    std::ofstream file_; // File to log to

    static std::unique_ptr<Logger> m_instance; // Singleton instance
};