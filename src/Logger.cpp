#include <common/Logger.h>

std::unique_ptr<Logger> Logger::m_instance = nullptr;

Logger::Logger(const std::filesystem::path& path)
{
    file_.open(path, std::ios::out | std::ios::app);
    if (!file_.is_open())
        throw std::runtime_error("Failed to open log file: ");
}

void Logger::Initialize(const std::filesystem::path& path)
{
    m_instance = std::unique_ptr<Logger>(new Logger(path));
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

}

void Logger::Print_(const std::string& message)
{
}
