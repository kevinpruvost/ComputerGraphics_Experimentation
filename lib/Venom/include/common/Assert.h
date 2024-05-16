#pragma once

#include <common/Logger.h>

#undef assert
#ifdef _DEBUG
#define assert(x) if (!(x)) { Logger::DebugPrint("Assertion failed: %s, File: %s, Line: %d\n", #x, __FILE__, __LINE__); exit(1); }
#else
#define assert(x)
#endif
