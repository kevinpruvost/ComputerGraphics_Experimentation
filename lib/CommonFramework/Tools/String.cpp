#include <common/String.h>

#include <algorithm>
#include <cstring>
#include "String.h"

void String::ToLower()
{
    std::transform(begin(), end(), begin(), ::tolower);
}

String::operator std::string()
{
    return *this;
}

ConcatString::ConcatString(const char* str1, const char* str2)
    : __str{ new char[strlen(str1) + strlen(str2) + 1] }
{
    strcpy(__str, str1);
    strcat(__str, str2);
}

ConcatString::ConcatString(const char* str1, const char* str2, size_t count)
    : __str{ new char[strlen(str1) + count + 1] }
{
    strcpy(__str, str1);
    strncat(__str, str2, count);
}

ConcatString::~ConcatString()
{
    delete __str;
}

ConcatString::operator const char* ()
{
    return __str;
}
