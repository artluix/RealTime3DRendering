#pragma once
#include "library/Common.h"

#include <string>
#include <vector>

namespace library
{
class Path
{
public:
	Path() = default;
	explicit Path(const std::string& s);
	explicit Path(std::string&& s);

	const std::string& GetString() const { return m_string; }
	const char* GetCString() const { return m_string.c_str(); }

	const std::wstring& GetWideString() const { return m_wstring; }
	const wchar_t* GetWideCString() const { return m_wstring.c_str(); }

	Path GetBaseName() const;
	Path GetDirName() const;

	Pair<Path> Split() const;

	Path& Join(const Path& other);
	Path& Join(const std::vector<Path>& paths);
	Path Join(const Path& other) const;
	Path Join(const std::vector<Path>& paths) const;

	Pair<Path> SplitExt() const;
	Path GetExt() const;

	Path& operator+=(const Path& p);
	Path operator+(const Path& p) const;

	explicit operator bool() const { return !m_string.empty(); }

	bool operator!=(const Path& other) const;
	bool operator==(const Path& other) const;
	bool operator<(const Path& other) const;
	bool operator>(const Path& other) const;

private:
	void FixSeparator();
	void UpdateCached(); // will update wstring

	std::string m_string;
	std::wstring m_wstring;
};

//-------------------------------------------------------------------------

inline bool Path::operator!=(const Path& other) const
{
	return m_string != other.m_string;
}

inline bool Path::operator==(const Path& other) const
{
	return m_string == other.m_string;
}

inline bool Path::operator<(const Path& other) const
{
	return m_string < other.m_string;
}

inline bool Path::operator>(const Path& other) const
{
	return m_string > other.m_string;
}
} // namespace library
