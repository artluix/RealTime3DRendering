#pragma once
#include "library/CommonTypes.h"
#include "library/Path.h"

namespace library::utils
{
	std::wstring ToWideString(const std::string& str);

	Path GetCurrentDirectory();
	Path GetExecutableDirectory();

	void LoadBinaryFile(const Path& path, std::vector<byte>& data);
} // namespace library::utils
