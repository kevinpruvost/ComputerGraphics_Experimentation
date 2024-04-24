#pragma once

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