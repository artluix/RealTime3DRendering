#pragma once
#include "library/Components/Component.h"

#include "library/CommonTypes.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	struct BufferData
	{
		ComPtr<ID3D11Buffer> buffer;
		unsigned count = 0;
	};

	//-------------------------------------------------------------------------

	class Material;
	class Effect;
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
		virtual const Effect* GetEffect() const { return nullptr; }

	protected:
		explicit DrawableComponent() = default;

		virtual Material* GetMaterial() { return nullptr; }
		virtual Effect* GetEffect() { return nullptr; }

		const CameraComponent* m_camera = nullptr;
		bool m_visible = true;
	};
} // namespace library
