#pragma once
#include <library/components/SceneComponent.h>
#include <library/DirectXForwardDeclarations.h>

#include <memory>
#include <functional>

namespace library
{
	class Effect;
	class KeyboardComponent;
	class DiffuseLightingMaterial;
} // namespace library

namespace demo
{
	class DiffuseLightingMaterialComponent : public library::SceneComponent
	{
		RTTI_CLASS(DiffuseLightingMaterialComponent, library::SceneComponent)

	public:
		explicit DiffuseLightingMaterialComponent(
			const library::Application& app,
			const library::CameraComponent& camera,
			const library::KeyboardComponent& keyboard
		);

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		void UpdateAmbientLight(const library::Time& time);
		void UpdateDirectionalLight(const library::Time& time);

		std::unique_ptr<library::Effect> m_effect;
		std::unique_ptr<library::DiffuseLightingMaterial> m_material;

		ComPtr<ID3D11ShaderResourceView> m_textureShaderResourceView;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;

		unsigned m_indicesCount;

		std::reference_wrapper<const library::KeyboardComponent> m_keyboard;
	};
} // namespace demo
