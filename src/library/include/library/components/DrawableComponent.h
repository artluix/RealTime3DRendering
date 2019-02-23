#pragma once
#include "library/components/Component.h"

namespace library
{
	class Material;
	class CameraComponent;

	class DrawableComponent : public virtual Component
	{
		RTTI_CLASS(DrawableComponent, Component)

	public:
		bool IsVisible() const { return m_visible; }
		void SetVisible(const bool visible);

		const CameraComponent* GetCamera() const { return m_camera; }
		void SetCamera(const CameraComponent& camera);

		virtual void Draw(const Time& time) {}

		virtual const Material* GetMaterial() const { return nullptr; }

	protected:
		explicit DrawableComponent() = default;

		virtual Material* GetMaterial() { return nullptr; }

	private:
		bool m_visible = true;

		const CameraComponent* m_camera = nullptr;
	};
} // namespace library
