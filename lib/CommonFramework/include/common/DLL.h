#pragma once

#include <iostream>
#include <string>
#include <filesystem>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#else
    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#endif

class DLL
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

