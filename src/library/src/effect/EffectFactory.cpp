#include "StdAfx.h"
#include "library/effect/EffectFactory.h"

#include "library/effect/Effect.h"
#include "library/Path.h"

#include <algorithm>
#include <memory>

namespace library
{
	std::map<std::string, EffectPtr> EffectFactory::s_effects;

	EffectPtr EffectFactory::Create(const Application& app, const Path& path)
	{
		const auto effectName = path.GetBaseName().SplitExt()[0].GetString();

		auto it = s_effects.find(effectName);
		if (it != s_effects.end())
			return it->second;

		auto effect = std::make_shared<Effect>(app, path);
		s_effects.emplace(effectName, effect);
		return effect;
	}

	void EffectFactory::Reset()
	{
		s_effects.clear();
	}
} // namespace library
