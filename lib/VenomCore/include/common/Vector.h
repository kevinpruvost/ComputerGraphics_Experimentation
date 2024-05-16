#pragma once

#include <array>
#include <vector>
#include <common/DLL.h>

template<typename val_type, size_t size>
class Vector : public std::array<val_type, size>
{
public:
};

typedef Vector<float, 3> Vec3f;
typedef Vec3f Vec3;

typedef std::vector<Vec3f> Vec3fArray;
typedef Vec3fArray Vec3Array;