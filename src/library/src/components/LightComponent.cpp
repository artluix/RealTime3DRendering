#include "StdAfx.h"
#include "library/components/LightComponent.h"

namespace library
{
	LightComponent::LightComponent(const Color& color /* = Color::White*/)
		: m_color(color)
	{
	}

	void LightComponent::SetColor(const Color& color)
	{
		if (m_color != color)
		{
			m_color = color;
		}
	}
} // namespace library