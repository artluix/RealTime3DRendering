#include "StdAfx.h"
#include "library/Path.h"

#include "library/Utils.h"

namespace library
{

namespace
{
constexpr char k_sep = '/';
constexpr char k_badSep = '\\';
} // namespace

//-------------------------------------------------------------------------

Path::Path(const std::string& s) : m_string(s)
{
	FixSeparator();
}

Path::Path(std::string&& s) : m_string(s)
{
	FixSeparator();
}

Path Path::GetBaseName() const
{
	return Split()[1];
}

Path Path::GetDirName() const
{
	return Split()[0];
}

Pair<Path> Path::Split() const
{
	if (m_string.empty())
		return Pair<Path>();

	const auto lastSlashIdx = m_string.find_last_of('/');
	if (lastSlashIdx != std::string::npos)
	{
		Pair<Path> result;
		result[0] = Path(m_string.substr(0, lastSlashIdx));

		if (lastSlashIdx < m_string.length() - 1)
			result[1] = Path(m_string.substr(lastSlashIdx + 1));

		return result;
	}
	else
	{
		return Pair<Path>(*this, Path());
	}
}

//-------------------------------------------------------------------------

Path& Path::Join(const Path& other)
{
	if (!other)
		return *this;

	const auto& otherString = other.GetString();

	const bool hasEndSlash = (m_string.back() == '/');
	const bool hasStartSlash = (otherString.front() == '/');

	if (hasStartSlash && hasEndSlash)
	{
		m_string.pop_back();
	}
	else if (!hasStartSlash && !hasEndSlash)
	{
		m_string += '/';
	}

	m_string += otherString;

	UpdateCached();

	return *this;
}

Path& Path::Join(const DynArray<Path>& paths)
{
	if (!paths.empty())
	{
		for (const auto& path : paths)
		{
			Join(path);
		}
	}

	return *this;
}

Path Path::Join(const Path& other) const
{
	auto path = *this;
	path.Join(other);
	return path;
}

Path Path::Join(const DynArray<Path>& paths) const
{
	auto path = *this;
	path.Join(paths);
	return path;
}

//-------------------------------------------------------------------------

Pair<Path> Path::SplitExt() const
{
	if (m_string.empty())
		return Pair<Path>();

	const auto lastDotIdx = m_string.find_last_of('.');
	if (lastDotIdx != std::string::npos)
	{
		Pair<Path> result;
		result[0] = Path(m_string.substr(0, lastDotIdx));

		if (lastDotIdx < m_string.length() - 1)
			result[1] = Path(m_string.substr(lastDotIdx + 1));

		return result;
	}
	else
	{
		return Pair<Path>(*this, Path());
	}
}

Path Path::GetExt() const
{
	return SplitExt()[1];
}

//-------------------------------------------------------------------------

Path& Path::operator+=(const Path& p)
{
	return Join(p);
}

Path Path::operator+(const Path& p) const
{
	return Join(p);
}

//-------------------------------------------------------------------------

void Path::FixSeparator()
{
	std::replace(std::begin(m_string), std::end(m_string), k_badSep, k_sep);
	UpdateCached();
}

void Path::UpdateCached()
{
	m_wstring = utils::ToWideString(m_string);
}
} // namespace library
