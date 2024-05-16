#include <common/Config.h>
#include <common/Error.h>
#include <ryml.hpp>
#include <c4/std/string.hpp>

#include <common/Resources.h>
#include <common/ShaderPipeline.h>
#include <common/Texture.h>
#include <common/Model.h>

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
    {
        auto ite = m_instances.find(path.string());
        if (ite != m_instances.end())
            m_instances.erase(ite);
    }
        
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
        settings.api = GraphicsEngineAPI::OpenGL;
    else if (api == "Vulkan")
        settings.api = GraphicsEngineAPI::Vulkan;
    else if (api == "DirectX12")
        settings.api = GraphicsEngineAPI::DirectX12;
    else
        throw NotImplementedException("Engine API mentioned in YAML config file not recognized. (has to be OpenGL/Vulkan/DirectX11)");
    engineNode["name"] >> settings.name;
    return settings;
}

Venom::ErrorCode Config::LoadResources() const
{
    // Get current path
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::current_path(currentPath / "resources");
    ryml::Tree root = *(ryml::Tree*)m_node;
    c4::yml::NodeRef resourcesNode = root["resources"];
    for (auto node : resourcesNode.children())
    {
        c4::csubstr name = node.key();
        if (name == "shaders")
        {
            for (const auto & shader : node.children())
            {
                const String shaderName = YamlNodeToString(shader["name"]);
                if (Resources::Load<ShaderPipeline>(shaderName.c_str(), shader) == nullptr)
                    return Venom::ErrorCode::Failure;
            }
        }
        else if (name == "textures")
        {
            for (const auto & texture : node.children())
            {
                const String textureName = YamlNodeToString(texture["name"]);
                if (Resources::Load<Texture>(textureName.c_str(), texture) == nullptr)
                    return Venom::ErrorCode::Failure;
            }
        }
        else if (name == "models")
        {
            for (const auto& model : node.children())
            {
                const String modelName = YamlNodeToString(model["name"]);
                if (Resources::Load<Model>(modelName.c_str(), model) == nullptr)
                    return Venom::ErrorCode::Failure;
            }
        }
    }
    std::filesystem::current_path(currentPath);
    return Venom::ErrorCode::Success;
}
