#include "StdAfx.h"
#include "library/Components/LightComponent.h"

namespace library
{
LightComponent::LightComponent(const Color& color /* = Color::White*/) : m_color(color)
{}

void LightComponent::SetColor(const Color& color)
{
	m_color = color;
}
} // namespace library
