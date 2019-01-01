#include "StdAfx.h"
#include "library/Path.h"

#include "library/Utils.h"

namespace library
{
	namespace fs
	{
		namespace
		{
			constexpr char k_sep = '/';
			constexpr char k_badSep = '\\';
		}

		Path::Path(const std::string& s)
			: m_string(s)
		{
			FixSeparator();
		}

		Path::Path(std::string&& s)
			: m_string(s)
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

		Tuple2<Path> Path::Split() const
		{
			if (m_string.empty())
				return {};

			const auto lastSlashIdx = m_string.find_last_of('/');
			if (lastSlashIdx != std::string::npos)
			{
				Tuple2<Path> result;
				result[0] = Path(m_string.substr(0, lastSlashIdx));

				if (lastSlashIdx < m_string.length() - 1)
					result[1] = Path(m_string.substr(lastSlashIdx + 1));

				return result;
			}
			else
			{
				return { *this, };
			}
		}

		Path& Path::Join(const Path& other)
		{
			const auto& otherString = other.GetString();

			const bool haveEndSlash = (m_string.back() == '/');
			const bool haveStartSlash = (otherString.front() == '/');

			if (haveStartSlash && haveEndSlash)
			{
				m_string.pop_back();
			}
			else if (!haveStartSlash && !haveEndSlash)
			{
				m_string += '/';
			}

			m_string += otherString;

			UpdateCached();

			return *this;
		}

		Path& Path::Join(const std::vector<Path>& paths)
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

		Tuple2<Path> Path::SplitExt() const
		{
			if (m_string.empty())
				return {};

			const auto lastDotIdx = m_string.find_last_of('.');
			if (lastDotIdx != std::string::npos)
			{
				Tuple2<Path> result;
				result[0] = Path(m_string.substr(0, lastDotIdx));

				if (lastDotIdx < m_string.length() - 1)
					result[1] = Path(m_string.substr(lastDotIdx + 1));

				return result;
			}
			else
			{
				return { *this, };
			}
		}

		Path Path::GetExt() const
		{
			return SplitExt()[1];
		}

		void Path::FixSeparator()
		{
			std::replace(std::begin(m_string), std::end(m_string), k_badSep, k_sep);
			UpdateCached();
		}

		void Path::UpdateCached()
		{
			m_wstring = utils::ToWideString(m_string);
		}

	} // namespace fs
} // namespace library
