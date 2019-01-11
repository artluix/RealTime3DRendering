#include "StdAfx.h"
#include "library/Renderer.h"

#include "library/components/DrawableComponent.h"
#include "library/components/UIComponent.h"
#include "library/components/SceneComponent.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectMaterial.h"

#include "library/Time.h"

#include <algorithm>
//#include <type_traits>

namespace library
{
	void Renderer::RegisterForRender(Drawable* const drawable)
	{
		auto it = std::find(m_drawables.begin(), m_drawables.end(), drawable);
		if (it != m_drawables.end())
			return;

		m_drawables.push_back(drawable);
	}

	void Renderer::UnregisterForRender(Drawable* const drawable)
	{
		auto it = std::find(m_drawables.begin(), m_drawables.end(), drawable);
		if (it == m_drawables.end())
			return;

		m_drawables.erase(it);
	}

	void Renderer::Render(const Time& time)
	{
		auto drawables = m_drawables;

		// 1. Separate 2D & 3D drawables
		auto sortByTypePred = [](const Drawable* lhs, const Drawable* rhs)
		{
			if (!!rhs->As<SceneComponent>() && !lhs->As<SceneComponent>())
				return true;

			return false;
		};
		std::sort(drawables.begin(), drawables.end(), sortByTypePred);

		// 2. Find Separate border
		auto uiPred = [](const Drawable* drawable)
		{
			return !!drawable->As<UIComponent>();
		};
		auto uiIt = std::find_if(drawables.begin(), drawables.end(), uiPred);

		// 3. Sort by effects 2D & 3D components
		auto sortByEffectPred = [](const Drawable* lhs, const Drawable* rhs)
		{
			const auto lhsMat = lhs->GetEffectMaterial();
			if (lhsMat)
				return false;

			const auto rhsMat = lhs->GetEffectMaterial();
			if (!rhsMat)
				return false;

			return lhsMat->GetEffect().GetName() < rhsMat->GetEffect().GetName();
		};
		std::sort(drawables.begin(), uiIt - 1, sortByEffectPred);
		std::sort(uiIt, drawables.end(), sortByEffectPred);

		// 4. Draw
		auto drawPred = [&time](Drawable* drawable)
		{
			drawable->Draw(time);
		};
		std::for_each(drawables.begin(), drawables.end(), drawPred);
	}
} // namespace library
