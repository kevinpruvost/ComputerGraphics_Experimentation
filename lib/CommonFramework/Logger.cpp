#include <common/Logger.h>

std::unique_ptr<Logger> Logger::m_instance = nullptr;

Logger::Logger(const std::filesystem::path& path)
{
    std::filesystem::create_directories(path.parent_path());
    file_.open(path, std::ios::app);
    if (!file_.is_open())
        throw std::runtime_error("Failed to open log file: ");
}

void Logger::Initialize(const std::filesystem::path& path)
{
    auto path_ = std::filesystem::path("log/") / path;
    m_instance = std::unique_ptr<Logger>(new Logger(path_));
}

void Logger::Reset(const std::filesystem::path& path)
{
    Destroy();
    Initialize(path);
}

void Logger::Destroy()
{
    m_instance.release();
}

void Logger::Log_(const std::string& message)
{
    file_ << message << std::endl;
}

void Logger::Print_(const std::string& message)
{
    std::cout << message << std::endl;
}