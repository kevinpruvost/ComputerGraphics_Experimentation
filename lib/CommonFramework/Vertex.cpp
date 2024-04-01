#include <common/Vertex.h>

Vertex::Vertex()
    : p{ 0.0f }
    , n{ 0.0f }
    , t{ 0.0f }
{
}

Vertex::Vertex(const glm::vec3 & position, const glm::vec3& normal, const glm::vec2& texCoord)
    : p{ position }
    , n{ normal }
    , t{ texCoord }
{

}

Vertex::~Vertex()
{
}
