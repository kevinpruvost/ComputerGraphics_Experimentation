#pragma once
#include <yaml-cpp/yaml.h>
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
    template<typename Key>
    const YAML::Node operator[](const Key& key) const { return m_node[key]; }
    template<typename Key>
    YAML::Node operator[](const Key& key) { return m_node[key]; }
    static Config* Get(const std::filesystem::path & path);
    WindowSettings WindowSettings() const;
    EngineSettings EngineSettings() const;

    ~Config();
private:
    Config(const std::string& filename);
    YAML::Node m_node;
    static std::unordered_map<std::string, std::unique_ptr<Config>> m_instances;
};

