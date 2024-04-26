#pragma once

#include <common/Error.h>
#include <common/Export.h>
#include <common/Format.h>

#include <exception>

class Exception : public std::exception
{
public:
    template <typename... Args>
    Exception(const fmt::format_string<Args...> str, Args&& ... args)
        : std::exception(fmt::vformat(str, fmt::make_format_args(args...)).c_str())
    {

    }
    Exception(const std::string & str)
        : std::exception(str.c_str())
    {

    }
    using std::exception::exception;
    COMMONFRAMEWORK_API ~Exception();
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