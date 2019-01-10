#pragma once
#include "library/fs/Path.h"

namespace library::util
{
	std::wstring ToWideString(const std::string& str);

	fs::Path GetCurrentDirectory();
	fs::Path GetExecutableDirectory();

	void LoadBinaryFile(const fs::Path& path, std::vector<byte>& data);

} // namespace library::util
