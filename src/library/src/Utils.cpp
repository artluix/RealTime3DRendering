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
			std::array<wchar_t, MAX_PATH> buffer;
			::GetCurrentDirectory(MAX_PATH, buffer.data());
			return filesystem::Path(buffer.data());
		}

		filesystem::Path GetExecutableDirectory()
		{
			std::array<wchar_t, MAX_PATH> buffer;
			::GetModuleFileName(nullptr, buffer.data(), MAX_PATH);
			return filesystem::Path(buffer.data()).GetDirName();
		}

		void LoadBinaryFile(const filesystem::Path& path, std::vector<byte>& data)
		{
			std::ifstream file(path.GetAsWideCString(), std::ios::binary);
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
