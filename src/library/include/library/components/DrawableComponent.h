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

		bool IsAutoRenderable() const { return m_isAutoRenderable; }
		void SetAutoRenderable(const bool autoRenderable);

		using Component::Initialize;

		virtual void Update(const Time& time);
		virtual void Draw(const Time& time) {}

		void AddToRenderer();
		void RemoveFromRenderer();

	protected:
		explicit DrawableComponent() = default;

	private:
		bool m_visible = true;
		bool m_isAutoRenderable = true;
	};
} // namespace library
