#pragma once

#include <common/Error.h>

#include <exception>
#include <format>

class Exception : public std::exception
{
public:
    template <typename... Args>
    Exception(const std::format_string<Args...> str, Args&& ... args)
        : std::exception(std::vformat(str.get(), std::make_format_args(args...)).c_str())
    {

    }
    Exception(const std::string & str)
        : std::exception(str.c_str())
    {

    }
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