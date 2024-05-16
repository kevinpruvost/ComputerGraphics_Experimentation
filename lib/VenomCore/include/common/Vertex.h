#pragma once


#include <common/Math_Base.h>
#include <common/DLL.h>
#include <common/Vector.h>
#include <common/Engine/EngineObject.h>

#include <vector>
#include <array>

class VENOM_API Vertex
{
public:
    Vertex();
    constexpr Vertex(const glm::vec3& __position, const glm::vec3& normal, const glm::vec2& texCoord);
    constexpr Vertex(const glm::vec3& __position, const glm::vec3& col);
    constexpr Vertex(const glm::vec3& __position);
    ~Vertex();

    glm::vec3 pos;
    union {
        glm::vec3 normals;
        glm::vec3 color;
    };
    glm::vec2 textureCoords;
};

typedef std::vector<Vertex> VertexArray;
typedef std::vector<unsigned int> IndexArray;

class VENOM_API VertexBuffer : public EngineObject
{
protected:
    VertexBuffer();

public:
    static VertexBuffer * CreateVertexBuffer();
    size_t GetVertexCount() const;
    size_t GetVertexRawSize() const;

public:
    virtual void SetVertices(const VertexArray& vertices) = 0;
    virtual void SetVertices(const VertexArray& vertices, const IndexArray & indices) = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void Draw() const = 0;
    const VertexArray & GetVertices() const;
    const IndexArray & GetIndices() const;

    void SetHasNormals(bool hasNormals);
    void SetHasTextureCoords(bool hasTextureCoords);
    bool HasNormals() const;
    bool HasTextureCoords() const;

protected:
    VertexArray _v;
    IndexArray _i;
    bool _hasNormals, _hasTextureCoords;
};