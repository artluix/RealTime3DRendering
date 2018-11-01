#include "library/Logger.h"

#include <cstdio>
#include <Windows.h>

namespace library
{

	const unsigned Logger::k_bufferSize = 256;

	void Logger::Log(const Level level, const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		Log(level, fmt, args);
		va_end(args);
	}

	void Logger::Log(const Level level, const char* fmt, const va_list ap)
	{
		char buffer[k_bufferSize];
		vsprintf_s(buffer, k_bufferSize, fmt, ap);

		std::string prefixMsg;
		switch (level)
		{
		case Level::Info:
			prefixMsg = "Info";
			break;

		case Level::Debug:
			prefixMsg = "Debug";
			break;

		case Level::Warning:
			prefixMsg = "Warn";
			break;

		case Level::Error:
			prefixMsg = "Error";
			break;

		default:
			break;
		}

		std::string message = prefixMsg + ": " + std::string(buffer);
		Message(message);
	}

	void Logger::Info(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		Log(Level::Info, fmt, args);
		va_end(args);
	}

	void Logger::Debug(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		Log(Level::Debug, fmt, args);
		va_end(args);
	}

	void Logger::Warn(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		Log(Level::Warning, fmt, args);
		va_end(args);
	}

	void Logger::Error(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		Log(Level::Error, fmt, args);
		va_end(args);
	}

	void Logger::Message(std::string msg, const bool newline /*= true*/)
	{
		if (newline)
			msg += '\n';

		OutputDebugStringA(msg.c_str());
	}

} // namespace library
