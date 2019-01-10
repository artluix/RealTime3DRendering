#include "StdAfx.h"
#include "library/effect/EffectFactory.h"

#include "library/effect/Effect.h"
#include "library/fs/Path.h"

#include <algorithm>
#include <memory>

namespace library
{
	namespace effect
	{
		std::map<std::string, EffectPtr> Factory::s_effects;

		EffectPtr Factory::Create(const Application& app, const fs::Path& path)
		{
			const auto effectName = path.GetBaseName().SplitExt()[0].GetString();

			auto it = s_effects.find(effectName);
			if (it != s_effects.end())
				return it->second;

			auto effect = std::make_shared<Effect>(app, path);
			s_effects.emplace(effectName, effect);
			return effect;
		}

		void Factory::Reset()
		{
			s_effects.clear();
		}

	} // namespace effect
} // namespace library
