#pragma once
#include "library/components/Component.h"
#include "library/Path.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class Material;
	class CameraComponent;

	class DrawableComponent : public Component
	{
		RTTI_CLASS(DrawableComponent, Component)

	public:
		bool IsVisible() const { return m_visible; }
		void SetVisible(const bool visible);

		const CameraComponent* GetCamera() const { return m_camera; }
		void SetCamera(const CameraComponent& camera);

		void Initialize() override;
		virtual void Draw(const Time& time) = 0;

	protected:
		explicit DrawableComponent(const Application& app);

	private:
		const CameraComponent* m_camera = nullptr;
		bool m_visible = true;
	};
} // namespace library
