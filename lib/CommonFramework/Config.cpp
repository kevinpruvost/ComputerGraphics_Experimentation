#include <common/Config.h>

std::unordered_map<std::string, std::unique_ptr<Config>> Config::m_instances;

Config::Config(const std::string & filename)
    : m_node(YAML::LoadFile(filename))
{
    if (m_node.IsNull())
        throw std::runtime_error("Failed to load config file: " + filename);

    const auto config_name = m_node["config_name"];
    if (!config_name.IsDefined())
        throw std::runtime_error("Config file must contain a 'config_name' field");
}

Config::~Config()
{
}

Config * Config::Load(const std::filesystem::path& path)
{
    Config* c = nullptr;
    m_instances[path.string()] = std::unique_ptr<Config>((c = new Config(path.string())));
    return c;
}

void Config::Reset(const std::filesystem::path& path)
{
    Load(path);
}

void Config::Destroy()
{
}

Config* Config::Get(const std::filesystem::path& path)
{
    return m_instances.at(path.string()).get();
}

WindowSettings Config::WindowSettings() const
{
    struct WindowSettings settings;
    auto windowNode = m_node["window_settings"];
    settings.name = windowNode["name"].as<std::string>();
    settings.width = windowNode["width"].as<int>();
    settings.height = windowNode["height"].as<int>();
    settings.resizable = windowNode["resizable"].as<bool>();
    settings.fullscreen = windowNode["fullscreen"].as<bool>();
    settings.vsync = windowNode["vsync"].as<bool>();
    settings.focused = windowNode["focused"].as<bool>();
    settings.refreshRate = windowNode["refreshRate"].as<int>();
    return settings;
}

EngineSettings Config::EngineSettings() const
{
    struct EngineSettings settings;
    auto engineNode = m_node["engine_settings"];
    settings.name = engineNode["name"].as<std::string>();
    return settings;
}
