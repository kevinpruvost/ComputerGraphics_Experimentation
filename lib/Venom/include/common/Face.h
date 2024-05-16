#pragma once

#include <common/Math_Base.h>
#include <common/DLL.h>

/// <summary>
/// We will only use Triangle faces
/// </summary>
template <size_t N>
class Face
{
public:
    VENOM_API Face()
        : v{ 0 }
    {}

    VENOM_API Face(unsigned int vertices[N])
        : v{ vertices }
    {}

    unsigned int v[N];
};

typedef Face<3> TriangleFace;
typedef Face<4> QuadFace;
typedef std::vector<TriangleFace> TriangleArray;