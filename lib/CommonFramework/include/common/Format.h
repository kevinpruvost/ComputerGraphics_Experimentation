#pragma once

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/xchar.h>
#include <fmt/ostream.h>

namespace Common
{
    template<typename ...Args>
    std::string format(const fmt::format_string<Args...> str, Args&& ... args)
    {
        return fmt::vformat(str, fmt::make_format_args(args...));
    }

    template<typename ...Args>
    std::string format(const fmt::wformat_string<Args...> str, Args&& ... args)
    {
        return fmt::vformat(str, fmt::make_format_args(args...));
    }
};