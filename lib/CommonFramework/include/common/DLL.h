#pragma once

#include <iostream>
#include <string>
#include <filesystem>

#ifdef _WIN32
#ifdef COMMONFRAMEWORK_EXPORTS
#define COMMONFRAMEWORK_API __declspec(dllexport)
#else
#define COMMONFRAMEWORK_API __declspec(dllimport)
#endif
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#else
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#endif

class COMMONFRAMEWORK_API DLL
{
public:
     explicit DLL(const std::filesystem::path& filePath);
     ~DLL();

    template<typename T>
    inline T getFunction(const char* functionName) const
    {
        return reinterpret_cast<T>(getFunction(functionName));
    }
    void* getFunction(const char* functionName) const;

private:
    void loadDLL(const std::filesystem::path& filePath);
    void unloadDLL();

    void * __dllHandle;
};

