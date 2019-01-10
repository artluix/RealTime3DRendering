#include "StdAfx.h"
#include "library/components/LightComponent.h"

namespace library
{
	namespace components
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

	} // namespace components
} // namespace library