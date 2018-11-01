#pragma once
#include "library/Common.h"

#include <string>
#include <vector>

namespace library
{
	namespace filesystem
	{

		class Path
		{
		public:
			explicit Path() = default;
			explicit Path(const std::string& s);
			explicit Path(const std::wstring& ws);
			explicit Path(std::string&& s);
			explicit Path(std::wstring&& ws);

			const std::wstring& GetAsWideString() const;
			const wchar_t* GetAsWideCString() const;

			Path GetBaseName() const;
			Path GetDirName() const;

			Tuple2<Path> Split() const;
			
			Path& Join(const Path& other);
			Path& Join(const std::vector<Path>& paths);

			Tuple2<Path> SplitExt() const;
			Path GetExt() const;

			bool operator != (const Path& other) const;
			bool operator == (const Path& other) const;

		private:
			void FixSeparator();

			static const wchar_t k_sep;

			std::wstring m_path;
		};

		//-------------------------------------------------------------------------

		inline bool Path::operator != (const Path& other) const
		{
			return m_path != other.m_path;
		}


		inline bool Path::operator == (const Path& other) const
		{
			return m_path == other.m_path;
		}

	} // namespace filesystem
} // namespace library
