#pragma once

#include <filesystem>
#include <common/String.h>
#include <c4/std/string.hpp>
#include <ryml.hpp>

using YamlNode = c4::yml::NodeRef;
using ConstYamlNode = c4::yml::ConstNodeRef;

String YamlNodeToString(const YamlNode& node);
String YamlNodeToString(const ConstYamlNode& node);
std::filesystem::path YamlNodeToPath(const YamlNode& node);
std::filesystem::path YamlNodeToPath(const ConstYamlNode& node);