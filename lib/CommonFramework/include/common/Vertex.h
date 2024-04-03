#pragma once


#include <common/Math_Base.h>
#include <common/DLL.h>
#include <common/Vector.h>

#include <vector>
#include <array>

class Vertex
{
public:
    COMMONFRAMEWORK_API Vertex();
    COMMONFRAMEWORK_API Vertex(const glm::vec3& __position, const glm::vec3& normal, const glm::vec2& texCoord);
    COMMONFRAMEWORK_API ~Vertex();

    glm::vec3 pos;
    glm::vec3 normals;
    glm::vec2 textureCoords;
};

typedef std::vector<Vertex> VertexArray;
