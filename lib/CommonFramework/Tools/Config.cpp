#include <common/Config.h>
#include <common/Error.h>
//#include <yaml-cpp/yaml.h>
#include <ryml.hpp>
#include <c4/std/string.hpp>

std::unordered_map<std::string, std::unique_ptr<Config>> Config::m_instances;

size_t file_get_contents(const char* filename, std::string * v)
{
    std::FILE* fp = std::fopen(filename, "rb");
    RYML_CHECK_MSG(fp != nullptr, "could not open file");
    std::fseek(fp, 0, SEEK_END);
    long sz = std::ftell(fp);
    v->resize(static_cast<typename std::string::size_type>(sz));
    if (sz)
    {
        std::rewind(fp);
        size_t ret = std::fread(&(*v)[0], 1, v->size(), fp);
        RYML_CHECK(ret == (size_t)sz);
    }
    std::fclose(fp);
    return v->size();
}

/** load a file from disk and return a newly created CharContainer */
std::string file_get_contents(const char* filename)
{
    std::string cc;
    file_get_contents(filename, &cc);
    return cc;
}

Config::Config(const std::string & filename)
    : m_node{nullptr}
{
    std::string contents = file_get_contents(filename.c_str());
    ryml::Tree doc = ryml::parse_in_arena(ryml::to_csubstr(contents));
    if (!doc.crootref().is_map())
        throw std::runtime_error("Failed to load config file: " + filename);

    // Access YAML nodes using RapidYAML
    const c4::yml::NodeRef config_name = doc["config_name"];
    if (!config_name.valid())
        throw std::runtime_error("Config file must contain a 'config_name' field");

    m_node = new ryml::Tree(doc);
}

Config::~Config()
{
    if (m_node) delete (ryml::Tree *)m_node;
}

Config * Config::Load(const std::filesystem::path& path)
{
    // Create Config instance
    auto c = new Config(path.string());

    // Add Config instance to instances map
    m_instances[path.string()] = std::unique_ptr<Config>(c);
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
    ryml::Tree root = *(ryml::Tree*)m_node;
    c4::yml::NodeRef windowNode = root["window_settings"];
    auto api = windowNode["api"].val();
    if (api == "GLFW")
        settings.api = WindowAPI::GLFW;
    else if (api == "WIN32")
        settings.api = WindowAPI::WindowsNative;
    else
        throw NotImplementedException("Window API mentioned in YAML config file not recognized. (has to be GLFW/WIN32)");
    windowNode["name"] >> settings.name;
    windowNode["height"] >> settings.height;
    windowNode["width"] >> settings.width;
    windowNode["resizable"] >> settings.resizable;
    windowNode["fullscreen"] >> settings.fullscreen;
    windowNode["vsync"] >> settings.vsync;
    windowNode["focused"] >> settings.focused;
    windowNode["refreshRate"] >> settings.refreshRate;
    return settings;
}

EngineSettings Config::EngineSettings() const
{
    struct EngineSettings settings;
    ryml::Tree root = *(ryml::Tree*)m_node;
    c4::yml::NodeRef engineNode = root["engine_settings"];
    auto api = engineNode["api"].val();
    if (api == "OpenGL")
        settings.api = EngineAPI::OpenGL;
    else if (api == "Vulkan")
        settings.api = EngineAPI::Vulkan;
    else if (api == "DirectX11")
        settings.api = EngineAPI::DirectX11;
    else
        throw NotImplementedException("Engine API mentioned in YAML config file not recognized. (has to be OpenGL/Vulkan/DirectX11)");
    engineNode["name"] >> settings.name;
    return settings;
}