#pragma once
#include "library/components/Component.h"

namespace library
{
	class CameraComponent;

	class DrawableComponent : public Component
	{
		RTTI_CLASS(DrawableComponent, Component)

	public:
		explicit DrawableComponent() = default;
		explicit DrawableComponent(const Application& app);
		explicit DrawableComponent(const Application& app, const CameraComponent& camera);
		~DrawableComponent() = default;

		bool IsVisible() const { return m_visible; }
		void SetVisible(const bool visible);

		const CameraComponent& GetCamera() { return *m_camera; }
		void SetCamera(const CameraComponent& camera);

		virtual void Draw(const Time& time) = 0;

	protected:
		bool m_visible = true;
		const CameraComponent* m_camera = nullptr;
	};

} // namespace library
