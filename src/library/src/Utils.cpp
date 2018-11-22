#include "StdAfx.h"
#include "library/Utils.h"

#include <Shlwapi.h>
#include <fstream>

namespace library
{
	namespace utils
	{
		std::wstring ToWideString(const std::string& str)
		{
			std::wstring result(std::cbegin(str), std::cend(str));
			return result;
		}

		filesystem::Path GetCurrentDirectory()
		{
			std::array<char, MAX_PATH> buffer;
			::GetCurrentDirectoryA(MAX_PATH, buffer.data());
			return filesystem::Path(buffer.data());
		}

		filesystem::Path GetExecutableDirectory()
		{
			std::array<char, MAX_PATH> buffer;
			::GetModuleFileNameA(nullptr, buffer.data(), MAX_PATH);
			return filesystem::Path(buffer.data()).GetDirName();
		}

		void LoadBinaryFile(const filesystem::Path& path, std::vector<byte>& data)
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
	} // namespace utils
} // namespace library
