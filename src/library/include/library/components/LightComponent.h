#pragma once
#include "library/Components/Component.h"
#include "library/Color.h"

namespace library
{
	class LightComponent : public virtual Component
	{
		RTTI_CLASS(LightComponent, Component)

	public:
		const Color& GetColor() const { return m_color; }
		void SetColor(const Color& color);

	protected:
		explicit LightComponent(const Color& color = Color::White);

		Color m_color;
	};
} // namespace library
