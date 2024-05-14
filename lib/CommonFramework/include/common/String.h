#pragma once

#include <string>

class String : public std::string
{
public:
    using std::string::string;
    using std::string::operator=;

    void ToLower();
    operator std::string();
};

class ConcatString
{
public:
    ConcatString(const char* str1, const char* str2);
    ConcatString(const char* str1, const char* str2, size_t count);
    ~ConcatString();
    operator const char *();
private:
    char* __str;
};