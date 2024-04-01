#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <common/Vector.h>
#include <array>

class Vertex
{
public:
    Vertex();
    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoord);
    ~Vertex();

    glm::vec3 p;
    glm::vec3 n;
    glm::vec2 t;
};

typedef std::vector<Vertex> VertexArray;
