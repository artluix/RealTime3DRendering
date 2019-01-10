#pragma once
#include "library/Common.h"

#include <string>
#include <vector>

namespace library::fs
{
	class Path
	{
	public:
		explicit Path() = default;
		explicit Path(const std::string& s);
		explicit Path(std::string&& s);

		const std::string& GetString() const { return m_string; }
		const char* GetCString() const { return m_string.c_str(); }

		const std::wstring& GetWideString() const { return m_wstring; }
		const wchar_t* GetWideCString() const { return m_wstring.c_str(); }

		Path GetBaseName() const;
		Path GetDirName() const;

		Tuple2<Path> Split() const;

		Path& Join(const Path& other);
		Path& Join(const std::vector<Path>& paths);

		Tuple2<Path> SplitExt() const;
		Path GetExt() const;

		bool operator != (const Path& other) const;
		bool operator == (const Path& other) const;
		bool operator < (const Path& other) const;
		bool operator > (const Path& other) const;

	private:
		void FixSeparator();
		void UpdateCached(); // will update wstring

		std::string m_string;
		std::wstring m_wstring;
	};

	//-------------------------------------------------------------------------

	inline bool Path::operator != (const Path& other) const
	{
		return m_string != other.m_string;
	}

	inline bool Path::operator == (const Path& other) const
	{
		return m_string == other.m_string;
	}

	inline bool Path::operator < (const Path& other) const
	{
		return m_string < other.m_string;
	}

	inline bool Path::operator > (const Path& other) const
	{
		return m_string > other.m_string;
	}

} // namespace library::fs
