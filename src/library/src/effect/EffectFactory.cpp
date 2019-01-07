#include "StdAfx.h"
#include "library/effect/EffectFactory.h"

#include "library/effect/Effect.h"
#include "library/Path.h"

#include <algorithm>
#include <memory>

namespace library
{
	std::map<std::string, Effect*> EffectFactory::s_effects;

	Effect& EffectFactory::Create(const Application& app, const fs::Path& path)
	{
		const auto effectName = path.GetBaseName().SplitExt()[0].GetString();

		auto it = s_effects.find(effectName);
		if (it != s_effects.end())
			return *it->second;

		auto effect = new Effect(app, path);
		s_effects.emplace(effectName, effect);
		return *effect;
	}

	void EffectFactory::Clear()
	{
		std::for_each(s_effects.begin(), s_effects.end(), std::default_delete<Effect>());
		s_effects.clear();
	}

} // namespace library
