#pragma once
#include "library/NonConstructible.hpp"

#include <map>

namespace library
{
	class Application;
	class Effect;

	namespace fs
	{
		class Path;
	} // namespace fs

	class EffectFactory
	{
	public:
		static Effect& Create(const Application& app, const fs::Path& path);
		static void Clear();

	private:
		static std::map<std::string, Effect*> s_effects;
	};

} // namespace library
