#pragma once


#include <common/Math_Base.h>
#include <common/DLL.h>
#include <common/Vector.h>
#include <common/Engine/EngineObject.h>

#include <vector>
#include <array>

class COMMONFRAMEWORK_API Vertex
{
public:
    Vertex();
    constexpr Vertex(const glm::vec3& __position, const glm::vec3& normal, const glm::vec2& texCoord);
    constexpr Vertex(const glm::vec3& __position);
    ~Vertex();

    glm::vec3 pos;
    glm::vec3 normals;
    glm::vec2 textureCoords;
};

typedef std::vector<Vertex> VertexArray;

class COMMONFRAMEWORK_API VertexBuffer : public EngineObject
{
protected:
    VertexBuffer();

public:
    static VertexBuffer * CreateVertexBuffer();
    size_t GetVertexCount() const;
    size_t GetVertexRawSize() const;

public:
    virtual void SetVertices(const VertexArray& vertices) = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    VertexArray GetVertices() const;

protected:
    VertexArray _v;
};