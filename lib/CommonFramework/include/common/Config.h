#pragma once
#include <filesystem>
#include <unordered_map>
#include <common/WindowSettings.h>
#include <common/EngineSettings.h>

class Config
{
public:
    static Config * Load(const std::filesystem::path& path);
    static void Reset(const std::filesystem::path& path);
    static void Destroy();
    static Config* Get(const std::filesystem::path & path);
    WindowSettings WindowSettings() const;
    EngineSettings EngineSettings() const;

    ~Config();
private:
    Config(const std::string& filename);
    void * m_node = nullptr;
    static std::unordered_map<std::string, std::unique_ptr<Config>> m_instances;
};

