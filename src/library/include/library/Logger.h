#pragma once
#include "library/NonConstructible.hpp"

#include <cstdarg>
#include <string>

namespace library
{
    struct Logger : public NonConstructible<Logger>
    {
        enum class Level
        {
            Info,
            Debug,
            Warning,
            Error,
        };

        static void Log(const Level level, const char* fmt, ...);

        static void Info(const char* fmt, ...);
        static void Debug(const char* fmt, ...);
        static void Warn(const char* fmt, ...);
        static void Error(const char* fmt, ...);

        static void Message(std::string msg, const bool newline = true);

    private:
        static void Log(const Level level, const char* fmt, const va_list ap);
    };
} // namespace library
