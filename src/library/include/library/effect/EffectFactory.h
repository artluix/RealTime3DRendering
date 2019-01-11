#pragma once
#include "library/NonConstructible.hpp"

#include <map>
#include <memory>

namespace library
{
	class Application;
	class Path;

	class Effect;
	using EffectPtr = std::shared_ptr<Effect>;

	class EffectFactory : public NonConstructible<EffectFactory>
	{
	public:
		static EffectPtr Create(const Application& app, const Path& path);
		static void Reset();

	private:
		static std::map<std::string, EffectPtr> s_effects;
	};
} // namespace library
