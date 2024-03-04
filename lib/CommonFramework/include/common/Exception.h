#pragma once

#include <common/Error.h>

#include <exception>

class Exception : public std::exception
{
public:
    using std::exception::exception;
    ~Exception();
};

