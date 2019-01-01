#pragma once
#include "library/components/BaseComponent.h"

namespace library
{
	class DrawableComponent : public rtti::Class<DrawableComponent, BaseComponent>
	{
	public:
		explicit DrawableComponent(const Application& app);
		~DrawableComponent() = default;

		bool IsVisible() const { return m_visible; }
		void SetVisible(const bool visible);

		virtual void Draw(const Time& time);

	protected:
		bool m_visible;
	};

} // namespace library
