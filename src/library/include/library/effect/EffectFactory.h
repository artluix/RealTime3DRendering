#pragma once
#include "library/NonConstructible.hpp"

#include <map>
#include <memory>

namespace library
{
	class Application;
	
	class Effect;
	using EffectPtr = std::shared_ptr<Effect>;

	namespace fs
	{
		class Path;
	} // namespace fs

	class EffectFactory
	{
	public:
		static EffectPtr Create(const Application& app, const fs::Path& path);
		static void Clear();

	private:
		static std::map<std::string, EffectPtr> s_effects;
	};

} // namespace library
