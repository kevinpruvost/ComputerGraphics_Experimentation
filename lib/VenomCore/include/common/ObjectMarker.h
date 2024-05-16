#pragma once

#include <common/ShaderPipeline.h>

class ObjectMarker
{
public:
    static void DrawMarker(const glm::vec3& position, const glm::vec3& scale = glm::vec3(1.0f));
    static void SetMarkerShaderPipeline(ShaderPipeline* shader);
};