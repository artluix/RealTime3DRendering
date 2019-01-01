#pragma once
#include "library/Path.h"

namespace library
{
	namespace utils
	{
		std::wstring ToWideString(const std::string& str);

		fs::Path GetCurrentDirectory();
		fs::Path GetExecutableDirectory();

		void LoadBinaryFile(const fs::Path& path, std::vector<byte>& data);
	}; // namespace utils

} // namespace library
