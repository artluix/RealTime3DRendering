#include "library/Path.h"

namespace library
{
    namespace filesystem
    {
        namespace
        {
            const wchar_t Sep = L'/';
        }

        Path::Path(const std::string& s)
            : m_path(std::cbegin(s), std::cend(s))
        {
            FixSeparator();
        }

        Path::Path(const std::wstring& ws)
            : m_path(ws)
        {
            FixSeparator();
        }

        Path::Path(std::string&& s)
            : m_path(std::cbegin(s), std::cend(s))
        {
            FixSeparator();
        }

        Path::Path(std::wstring&& ws)
            : m_path(std::forward<std::wstring>(ws))
        {
            FixSeparator();
        }

        const std::wstring& Path::GetAsWideString() const
        {
            return m_path;
        }

        const wchar_t* Path::GetAsWideCString() const
        {
            return m_path.c_str();
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
            if (m_path.empty())
                return {};

            const auto lastSlashIdx = m_path.find_last_of(L"/");
            if (lastSlashIdx != std::wstring::npos)
            {
                Tuple2<Path> result;
                result[0] = Path(m_path.substr(0, lastSlashIdx));

                if (lastSlashIdx < m_path.length() - 1)
                    result[1] = Path(m_path.substr(lastSlashIdx + 1));

                return result;
            }
            else
            {
                return { *this, };
            }
        }

        Path& Path::Join(const Path& other)
        {
            const auto& otherWString = other.GetAsWideString();

            const bool haveEndSlash = m_path.back() == L'/';
            const bool haveStartSlash = otherWString.front() == L'/';

            if (haveStartSlash && haveEndSlash)
            {
                m_path.pop_back();
            }
            else if (!haveStartSlash && !haveEndSlash)
            {
                m_path += L"/";
            }

            m_path += otherWString;
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
            if (m_path.empty())
                return {};

            const auto lastDotIdx = m_path.find_last_of(L".");
            if (lastDotIdx != std::wstring::npos)
            {
                Tuple2<Path> result;
                result[0] = Path(m_path.substr(0, lastDotIdx));

                if (lastDotIdx < m_path.length() - 1)
                    result[1] = Path(m_path.substr(lastDotIdx + 1));

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
            static const wchar_t badSep = L'\\';
            std::replace(std::begin(m_path), std::end(m_path), badSep, Sep);
        }
    }
}
