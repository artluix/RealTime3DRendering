#include "StdAfx.h"
#include "library/components/LightComponent.h"

namespace library
{
	LightComponent::LightComponent(const Application& app)
		: Class(app)
		, m_color(colors::White)
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
