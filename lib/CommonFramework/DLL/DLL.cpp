#include <common/DLL.h>
#include <common/Exception.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

DLL::DLL(const std::filesystem::path& filePath)
    : __dllHandle(nullptr)
{
    loadDLL(filePath);
}

DLL::~DLL() {
    unloadDLL();
}

void* DLL::getFunction(const char* functionName)
{
    if (!__dllHandle) {
        throw DLLException("Failed to get function from DLL: DLL not loaded");
    }

    {
        std::unordered_map<const char*, void*>::const_iterator ite = __functions.find(functionName);
        if (ite != __functions.end()) {
            return ite->second;
        };
    }

#ifdef _WIN32
    void* funcPtr = reinterpret_cast<void*>(GetProcAddress((HMODULE)__dllHandle, functionName));
    if (!funcPtr) {
        throw DLLException("Failed to get function pointer: {}", functionName);
    }
#else
    void* funcPtr = dlsym(dllHandle, functionName);
    if (!funcPtr) {
        throw DLLException(dlerror());
    }
#endif
    __functions[functionName] = funcPtr;
    return funcPtr;
}

void DLL::loadDLL(const std::filesystem::path& filePath) {
#ifdef _WIN32
    __dllHandle = LoadLibraryA(filePath.string().c_str());
    if (!__dllHandle) {
        throw RuntimeException("Failed to load DLL: {}: Error code: {}", filePath.string().c_str(), (int)GetLastError());
    }
#else
    dllHandle = dlopen(filePath.string().c_str(), RTLD_LAZY);
    if (!dllHandle) {
        throw std::runtime_error(dlerror());
    }
#endif
}

void DLL::unloadDLL() {
    if (__dllHandle) {
#ifdef _WIN32
        FreeLibrary(reinterpret_cast<HMODULE>(__dllHandle));
#else
        dlclose(__dllHandle);
#endif
        __dllHandle = nullptr;
    }
}