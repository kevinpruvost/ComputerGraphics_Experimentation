#include <common/Vertex.h>
#include <common/Engine/EngineLoader.h>

Vertex::Vertex()
    : pos{ 0.0f }
    , normals{ 0.0f }
    , textureCoords{ 0.0f }
{
}

constexpr Vertex::Vertex(const glm::vec3& __position, const glm::vec3& normal, const glm::vec2& texCoord, const glm::vec3& t, const glm::vec3& b)
    : pos{ __position }
    , normals{ normal }
    , textureCoords{ texCoord }
    , tangent{ t }
    , bitangent{ b }
{
}

constexpr Vertex::Vertex(const glm::vec3 & __position, const glm::vec3& normal, const glm::vec2& texCoord)
    : pos{ __position }
    , normals{ normal }
    , textureCoords{ texCoord }
{

}

constexpr Vertex::Vertex(const glm::vec3& __position, const glm::vec3& col)
    : pos{ __position }
    , color{ col }
    , textureCoords{ 0.0f }
{
}

constexpr Vertex::Vertex(const glm::vec3& __position)
    : pos{ __position }
    , normals{ 0.0f }
    , textureCoords{ 0.0f }
{
}

Vertex::~Vertex()
{
}

VertexBuffer::VertexBuffer()
    : EngineObject(EngineObjectType::Object)
    , _hasNormals{ false }
    , _hasTextureCoords{ false }
    , _hasTangentsAndBitangents{ false }
{
}

typedef VertexBuffer* (*CreateVertexBufferFn)();

VertexBuffer* VertexBuffer::CreateVertexBuffer()
{
    CreateVertexBufferFn createVertexBufferFn = EngineLoader::GetEngineDll()->getFunction<CreateVertexBufferFn>("createVertexBuffer");
    assert(createVertexBufferFn != nullptr);
    VertexBuffer* VertexBuffer = createVertexBufferFn();
    assert(VertexBuffer != nullptr);
    return VertexBuffer;
}

size_t VertexBuffer::GetVertexCount() const
{
    return _v.size();
}

size_t VertexBuffer::GetVertexRawSize() const
{
    return _v.size() * sizeof(Vertex);
}

const VertexArray & VertexBuffer::GetVertices() const
{
    return _v;
}

const IndexArray& VertexBuffer::GetIndices() const
{
    return _i;
}

void VertexBuffer::SetHasNormals(bool hasNormals)
{
    _hasNormals = hasNormals;
}

void VertexBuffer::SetHasTextureCoords(bool hasTextureCoords)
{
    _hasTextureCoords = hasTextureCoords;
}

void VertexBuffer::SetHasTangentsAndBitangents(bool hasTangentsAndBitangents)
{
    _hasTangentsAndBitangents = hasTangentsAndBitangents;
}

bool VertexBuffer::HasNormals() const
{
    return _hasNormals;
}

bool VertexBuffer::HasTextureCoords() const
{
    return _hasTextureCoords;
}

bool VertexBuffer::HasTangentsAndBitangents() const
{
    return _hasTangentsAndBitangents;
}
