#pragma once
#include "library/Components/Component.h"

namespace library
{
class Material;

// abstract drawable
class DrawableComponent : public virtual Component
{
	RTTI_CLASS(DrawableComponent, Component)

public:
	bool IsVisible() const { return m_visible; }
	void SetVisible(const bool visible) { m_visible = visible; }

	virtual void Draw(const Time& time) = 0;
	virtual const Material* GetMaterial() const { return nullptr; }

protected:
	DrawableComponent() = default;

	virtual Material* GetMaterial() { return nullptr; }

private:
	bool m_visible = true;
};
} // namespace library
