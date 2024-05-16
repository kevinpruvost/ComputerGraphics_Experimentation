#pragma once

#include <common/Mesh.h>

class Mesh_OGL : public Mesh
{
public:
    Venom::ErrorCode ReloadObjectFromEngine() override;
};

extern "C"
{
    EXPORT Mesh* createMesh();
}