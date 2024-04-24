#pragma once

#include <common/DLL.h>
#include <common/Memory.h>

class FrameworkLoader;

/**
 * @brief Don't forget to define extern function:
 * extern "C"
 * {
 *     EXPORT Rendering * DLL_SINGLETON_LOADING_NAME(Rendering)();
 * }
 */
class COMMONFRAMEWORK_API Rendering : public DLL_Singleton<Rendering>
{
public:
    Rendering();

    static inline void SetDepthTest(bool enable) { _instance->_SetDepthTest(enable); }

protected:
    virtual void _SetDepthTest(bool enable) const = 0;
};