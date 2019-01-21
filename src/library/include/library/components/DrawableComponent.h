#pragma once
#include "library/components/Component.h"

namespace library
{
	class DrawableComponent : public Component
	{
		RTTI_CLASS(DrawableComponent, Component)

	public:
		bool IsVisible() const { return m_visible; }
		void SetVisible(const bool visible);

		void Initialize() override;
		virtual void Draw(const Time& time) {}

	protected:
		explicit DrawableComponent(const Application& app);

	private:
		bool m_visible = true;
	};
} // namespace library
