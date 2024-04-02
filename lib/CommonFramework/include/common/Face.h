#pragma once

#include <common/Math_Base.h>

/// <summary>
/// We will only use Triangle faces
/// </summary>
template <size_t N>
class Face
{
public:
    Face()
        : v{ 0 }
    {}

    Face(unsigned int vertices[N])
        : v{ vertices }
    {}

    unsigned int v[N];
};

typedef Face<3> TriangleFace;
typedef Face<4> QuadFace;
typedef std::vector<TriangleFace> TriangleArray;