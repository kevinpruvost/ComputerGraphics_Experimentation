#pragma once

#include <common/Format.h>
#include <common/Export.h>
#include <mutex>
#include <memory>
#include <filesystem>
#include <array>
#include <unordered_set>

template<typename... Args>
std::string inline format(const std::string& fmt, Args... args) {
    const char* buf;
    sprintf(buf, fmt.c_str(), args...);
    return std::string(buf);
}

class VENOM_API Logger
{
public:
    static void Initialize(const std::filesystem::path & path);
    static void Reset(const std::filesystem::path& path);
    static void Destroy();
    static inline void Log(const std::string& message) { m_instance->Log_(message); m_instance->Print("[LOG]" + message); }
    static inline void Print(const std::string& message) { m_instance->Print_(message); }
    static Logger * Get() { return m_instance.get(); }

    // Function to log with format strings
    template<typename... Args>
    static inline void Log(const char * format, Args... args) {
        int length = snprintf(nullptr, 0, format, args...);
        std::string message(length+1, 0);
        sprintf(const_cast<char *>(message.c_str()), format, args...);

        // Log the message
        Log(message);
    }

    // Function to print with format strings
    template<typename... Args>
    static inline void Print(const char * format, Args... args) {
        int length = snprintf(NULL, 0, format, args...);
        std::string message(length+1, 0);
        sprintf(const_cast<char *>(message.c_str()), format, args...);

        // Print the message
        Print(message);
    }

    // Function to print with format strings
    template<typename... Args>
    static inline void DebugLog(const char * format, Args... args) {
#ifdef _DEBUG
        std::string msg = "[DEBUG]: ";
        msg += format;
        Log(msg.c_str(), args...);
#endif
    }

    // Function to print with format strings
    template<typename... Args>
    static inline void DebugPrint(const char * format, Args... args) {
#ifdef _DEBUG
        std::string msg = "[DEBUG]: ";
        msg += format;
        Print(msg.c_str(), args...);
#endif
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