#include "Rendering.h"

EXPORT Rendering* DLL_SINGLETON_LOADING_NAME(Rendering)()
{
    return new Rendering_OGL();
}