#pragma once


#include <common/Math_Base.h>

#include <vector>
#include <common/Vector.h>
#include <array>

class Vertex
{
public:
    Vertex();
    Vertex(const glm::vec3& __position, const glm::vec3& normal, const glm::vec2& texCoord);
    ~Vertex();

    glm::vec3 pos;
    glm::vec3 normals;
    glm::vec2 textureCoords;
};

typedef std::vector<Vertex> VertexArray;
