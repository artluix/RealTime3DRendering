#include "StdAfx.h"
#include "library/Components/LightComponent.h"

namespace library
{
LightComponent::LightComponent(const math::Color& color /* = colors::White*/)
	: m_color(color)
{}

void LightComponent::SetColor(const math::Color& color)
{
	m_color = color;
}
} // namespace library
