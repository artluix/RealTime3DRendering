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

	void Update(const Time& time) override {}

protected:
	explicit LightComponent(const Color& color = Color::White);
	
	void InitializeInternal() override {}

	Color m_color;
};
} // namespace library
