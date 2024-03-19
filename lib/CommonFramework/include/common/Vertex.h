#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <common/Vector.h>
#include <array>

class Vertex
{
public:
    Vertex(const glm::vec3 & position = glm::vec3(),
           const glm::vec3 & normals = glm::vec3(),
           const glm::vec2 & texCoords = glm::vec2());
    ~Vertex();

    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

typedef std::vector<Vertex> VertexArray;