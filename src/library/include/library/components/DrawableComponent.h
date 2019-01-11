#pragma once
#include "library/components/Component.h"
#include "library/Path.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class EffectMaterial;
	class CameraComponent;

	class DrawableComponent : public Component
	{
		RTTI_CLASS(DrawableComponent, Component)

	public:
		bool IsVisible() const { return m_visible; }
		void SetVisible(const bool visible);

		const CameraComponent* GetCamera() const { return m_camera; }
		void SetCamera(const CameraComponent& camera);

		const Path& GetModelPath() const { return m_modelPath; }
		void SetModelPath(const Path& path);

		virtual const EffectMaterial* GetEffectMaterial() const { return nullptr; }

		void Initialize() override;
		virtual void Draw(const Time& time);

	protected:
		explicit DrawableComponent(const Application& app);

		virtual EffectMaterial* GetEffectMaterial() { return nullptr; }

		// render stages
		virtual void SetIA();
		virtual void SetEffectData();
		virtual void Render();

		const CameraComponent* m_camera = nullptr;
		bool m_visible = true;

		Path m_modelPath;

		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;

		unsigned m_indicesCount = 0;
	};
} // namespace library
