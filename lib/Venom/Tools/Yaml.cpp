#include <common/Yaml.h>

String YamlNodeToString(const YamlNode& node)
{
    auto strNode = node.val();
    return String(strNode.str, strNode.len);
}

String YamlNodeToString(const ConstYamlNode& node)
{
    auto strNode = node.val();
    return String(strNode.str, strNode.len);
}

std::filesystem::path YamlNodeToPath(const YamlNode& node)
{
    auto strNode = node.val();
    return std::string(strNode.str, strNode.len);
}

std::filesystem::path YamlNodeToPath(const ConstYamlNode& node)
{
    auto strNode = node.val();
    return std::string(strNode.str, strNode.len);
}

float YamlNodeToFloat(const YamlNode& node)
{
    float f;
    node >> f;
    return f;
}

float YamlNodeToFloat(const ConstYamlNode& node)
{
    float f;
    node >> f;
    return f;
}

int YamlNodeToInt(const YamlNode& node)
{
    int f;
    node >> f;
    return f;
}

int YamlNodeToInt(const ConstYamlNode& node)
{
    int f;
    node >> f;
    return f;
}
