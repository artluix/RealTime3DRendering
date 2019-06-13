#pragma once
#include "library/Components/Component.h"
#include "library/math/Color.h"

namespace library
{
class LightComponent : public virtual Component
{
	RTTI_CLASS(LightComponent, Component)

public:
	const math::Color& GetColor() const { return m_color; }
	void SetColor(const math::Color& color);

	void Update(const Time& time) override {}

protected:
	explicit LightComponent(const math::Color& color = colors::White);
	
	void InitializeInternal() override {}

	math::Color m_color;
};
} // namespace library
