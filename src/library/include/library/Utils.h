#pragma once
#include "library/Path.h"

namespace library
{
	namespace utils
	{
		std::wstring ToWideString(const std::string& str);

		filesystem::Path GetCurrentDirectory();
		filesystem::Path GetExecutableDirectory();

		void LoadBinaryFile(const filesystem::Path& path, std::vector<byte>& data);
	}; // namespace utils
} // namespace library
