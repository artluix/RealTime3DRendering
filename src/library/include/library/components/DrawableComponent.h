#pragma once
#include "library/components/Component.h"

namespace library
{
	class DrawableComponent : public Component
	{
		RTTI_CLASS(DrawableComponent, Component)

	public:
		explicit DrawableComponent(const Application& app);

		bool IsVisible() const { return m_visible; }
		void SetVisible(const bool visible);

		virtual void Draw(const Time& time) = 0;

	protected:
		bool m_visible = true;
	};

} // namespace library
