#pragma once

#include <common/Error.h>

#include <exception>

class Exception : public std::exception
{
public:
    using std::exception::exception;
    ~Exception();
};

class NotImplementedException : public Exception
{
public:
    using Exception::Exception;
};

class RuntimeException : public Exception
{
public:
    using Exception::Exception;
};

class DLLException : public Exception
{
    using Exception::Exception;
};

class InvalidParameterException : public Exception
{
    using Exception::Exception;
};