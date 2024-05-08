#include <common/Rendering.h>

template<typename T>
UPtr<T> DLL_Singleton<T>::_instance;
//INITIALIZE_DLL_SINGLETON_INSTANCE(Rendering);

Drawable3D::DrawMode Rendering::_globalDrawMode = Drawable3D::DrawMode::SOLID;
Drawable3D::DrawMode Rendering::_lastDrawMode = Drawable3D::DrawMode::NONE;

Rendering::Rendering()
{
}
