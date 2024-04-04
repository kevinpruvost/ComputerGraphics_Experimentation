#include <common/Config.h>
#include <common/Error.h>
#include <yaml-cpp/yaml.h>

std::unordered_map<std::string, std::unique_ptr<Config>> Config::m_instances;

static inline YAML::Node& node_ptr(void * node)
{
    return *(YAML::Node *)(node);
}

Config::Config(const std::string & filename)
    : m_node(new YAML::Node(YAML::LoadFile(filename)))
{
    if (node_ptr(m_node).IsNull())
        throw std::runtime_error("Failed to load config file: " + filename);

    const auto config_name = node_ptr(m_node)["config_name"];
    if (!config_name.IsDefined())
        throw std::runtime_error("Config file must contain a 'config_name' field");
}

Config::~Config()
{
    if (m_node) delete (YAML::Node *)m_node;
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
    auto windowNode = node_ptr(m_node)["window_settings"];
    auto api = windowNode["api"].as<std::string>();
    if (api == "GLFW")
        settings.api = WindowAPI::GLFW;
    else if (api == "WIN32")
        settings.api = WindowAPI::WindowsNative;
    else
        throw NotImplementedException("Window API mentioned in YAML config file not recognized. (has to be GLFW/WIN32)");
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
    auto engineNode = node_ptr(m_node)["engine_settings"];
    auto api = engineNode["api"].as<std::string>();
    if (api == "OpenGL")
        settings.api = EngineAPI::OpenGL;
    else if (api == "Vulkan")
        settings.api = EngineAPI::Vulkan;
    else if (api == "DirectX11")
        settings.api = EngineAPI::DirectX11;
    else
        throw NotImplementedException("Engine API mentioned in YAML config file not recognized. (has to be OpenGL/Vulkan/DirectX11)");
    settings.name = engineNode["name"].as<std::string>();
    return settings;
}
