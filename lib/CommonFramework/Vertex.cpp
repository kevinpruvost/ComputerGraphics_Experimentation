#include <common/Vertex.h>

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normals, const glm::vec2& texCoords)
    : pos{ position }
    , normal{ normals }
    , texCoord{ texCoords }
{
}

Vertex::~Vertex()
{
}
