#include "StdAfx.h"
#include "library/Utils.h"

#include <Shlwapi.h>
#include <fstream>

namespace library::utils
{
	namespace
	{
		constexpr std::size_t k_maxPath = 255;
	}

	std::wstring ToWideString(const std::string& str)
	{
		std::wstring result(std::cbegin(str), std::cend(str));
		return result;
	}

	Path GetCurrentDirectory()
	{
		std::array<char, k_maxPath> buffer;
		::GetCurrentDirectoryA(k_maxPath, buffer.data());
		return Path(buffer.data());
	}

	Path GetExecutableDirectory()
	{
		std::array<char, k_maxPath> buffer;
		::GetModuleFileNameA(nullptr, buffer.data(), k_maxPath);
		return Path(buffer.data()).GetDirName();
	}

	void LoadBinaryFile(const Path& path, std::vector<byte>& data)
	{
		std::ifstream file(path.GetWideString(), std::ios::binary);
		if (file.bad())
		{
			throw std::exception("Could not open file.");
		}

		file.seekg(0, std::ios::end);
		const auto size = file.tellg();

		if (size > 0)
		{
			data.resize(std::size_t(size));
			file.seekg(0, std::ios::beg);
			file.read(reinterpret_cast<char*>(data.data()), size);
		}

		file.close();
	}
} // namespace library::utils
