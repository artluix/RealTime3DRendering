#pragma once
#include <library/components/SceneComponent.h>
#include <library/DirectXForwardDeclarations.h>

namespace library
{
	class KeyboardComponent;
	class Effect;
} // namespace library

namespace demo
{
	class TextureMappingMaterial;

	class TextureMappingMaterialComponent : public library::rtti::Class<TextureMappingMaterialComponent, library::SceneComponent>
	{
	public:
		explicit TextureMappingMaterialComponent(
			const library::Application& app,
			const library::CameraComponent& camera,
			const library::KeyboardComponent& keyboard
		);

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		std::unique_ptr<library::Effect> m_effect;
		std::unique_ptr<TextureMappingMaterial> m_material;

		ComPtr<ID3D11ShaderResourceView> m_textureShaderResourceView;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;

		unsigned m_indicesCount;

		std::reference_wrapper<const library::KeyboardComponent> m_keyboard;
	};

} // namespace demo
