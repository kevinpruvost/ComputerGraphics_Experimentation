#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <common/Constants.h>
#include <common/Logger.h>
#include <common/Memory.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#else
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#endif

#ifdef COMMONFRAMEWORK_EXPORTS
#define COMMONFRAMEWORK_API EXPORT
#else
#define COMMONFRAMEWORK_API IMPORT
#endif

#ifdef CONTEXT_EXPORTS
#define CONTEXT_API EXPORT
#else
#define CONTEXT_API IMPORT
#endif

class COMMONFRAMEWORK_API DLL
{
public:
     explicit DLL(const std::filesystem::path& filePath);
     ~DLL();

    template<typename T>
    inline T getFunction(const char* functionName)
    {
        return reinterpret_cast<T>(getFunction(functionName));
    }
    void* getFunction(const char* functionName);

private:
    void loadDLL(const std::filesystem::path& filePath);
    void unloadDLL();

    std::unordered_map<const char *, void *> __functions;
    void * __dllHandle;
};

class FrameworkLoader;

#define DLL_SINGLETON_LOADING_NAME(T) create##T##Instance
#define RELOAD_DLL_SINGLETON(T, dll) T::ReloadInstance(dll, _STRINGIZE(DLL_SINGLETON_LOADING_NAME(T)))
#define INITIALIZE_DLL_SINGLETON_INSTANCE(T) UPtr<T> DLL_Singleton<T>::_instance = nullptr

/**
 * @brief DO NOT FORGET TO INITALIZE INSTANCE WITH MACRO:
 * INITIALIZE_DLL_SINGLETON_INSTANCE(T)
 * @tparam T 
 */
template<typename T>
class COMMONFRAMEWORK_API DLL_Singleton
{
protected:
    friend class FrameworkLoader;
    static Venom::ErrorCode ReloadInstance(DLL * dll, const char * loadingName) {
        typedef T *(*FunctionLoader)();
        FunctionLoader functionPtr = dll->getFunction<FunctionLoader>(loadingName);
        if (functionPtr == nullptr)
        {
            Logger::Log("Failed to load instance of %s", loadingName);
            return Venom::ErrorCode::Failure;
        }
        T * instance = functionPtr();
        if (instance == nullptr)
        {
            Logger::Log("Failed to load instance of %s", loadingName);
            return Venom::ErrorCode::Failure;
        }
        _instance.reset(instance);
    }
    static UPtr<T> _instance;
};