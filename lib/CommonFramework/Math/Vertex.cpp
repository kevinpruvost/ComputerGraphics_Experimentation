#include <common/Vertex.h>

Vertex::Vertex()
    : pos{ 0.0f }
    , normals{ 0.0f }
    , textureCoords{ 0.0f }
{
}

Vertex::Vertex(const glm::vec3 & __position, const glm::vec3& normal, const glm::vec2& texCoord)
    : pos{ __position }
    , normals{ normal }
    , textureCoords{ texCoord }
{

}

Vertex::~Vertex()
{
}
