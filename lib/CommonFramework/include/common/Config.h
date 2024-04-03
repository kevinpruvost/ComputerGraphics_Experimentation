#pragma once

#include <filesystem>
#include <unordered_map>
#include <common/WindowSettings.h>
#include <common/EngineSettings.h>
#include <common/DLL.h>

class Config
{
public:
    COMMONFRAMEWORK_API static Config * Load(const std::filesystem::path& path);
    COMMONFRAMEWORK_API static void Reset(const std::filesystem::path& path);
    COMMONFRAMEWORK_API static void Destroy();
    COMMONFRAMEWORK_API static Config* Get(const std::filesystem::path & path);
    COMMONFRAMEWORK_API WindowSettings WindowSettings() const;
    COMMONFRAMEWORK_API EngineSettings EngineSettings() const;

    COMMONFRAMEWORK_API ~Config();
private:
    Config(const std::string& filename);
    void * m_node = nullptr;
    static std::unordered_map<std::string, std::unique_ptr<Config>> m_instances;
};

