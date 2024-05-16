#pragma once

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#else
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#endif

#ifdef VENOM_EXPORTS
#define VENOM_API EXPORT
#else
#define VENOM_API IMPORT
#endif

#ifdef CONTEXT_EXPORTS
#define VENOM_CONTEXT_API EXPORT
#else
#define VENOM_CONTEXT_API IMPORT
#endif