#include "Mesh.h"

// Export the factory function to create an instance of the class
EXPORT Mesh* createMesh() {
    return new Mesh_OGL();
}

Venom::ErrorCode Mesh_OGL::ReloadObjectFromEngine()
{
    return Venom::ErrorCode::Success;
}
