#include "StdAfx.h"
#include "library/lights/Light.h"

namespace library
{
	Light::Light(const Color& color /* = colors::White*/)
		: m_color(color)
	{
	}

	void Light::SetColor(const Color& color)
	{
		if (m_color != color)
		{
			m_color = color;
		}
	}

} // namespace library