#pragma once
#include "library/components/Component.h"

namespace library
{
	class DrawableComponent : public virtual Component
	{
		RTTI_CLASS(DrawableComponent, Component)

	public:
		bool IsVisible() const { return m_visible; }
		void SetVisible(const bool visible);

		void Initialize(const Application& app) override;
		virtual void Draw(const Time& time) {}

	protected:
		explicit DrawableComponent() = default;

	private:
		bool m_visible = true;
	};
} // namespace library
