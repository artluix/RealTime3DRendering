#pragma once
#include "library/NonConstructible.hpp"

#include <map>
#include <memory>

namespace library
{
	class Application;
	
	namespace fs
	{
		class Path;
	} // namespace fs

	namespace effect
	{
		class Effect;
		using EffectPtr = std::shared_ptr<Effect>;

		class Factory : public NonConstructible<Factory>
		{
		public:
			static EffectPtr Create(const Application& app, const fs::Path& path);
			static void Reset();

		private:
			static std::map<std::string, EffectPtr> s_effects;
		};

	} // namespace effect
} // namespace library
