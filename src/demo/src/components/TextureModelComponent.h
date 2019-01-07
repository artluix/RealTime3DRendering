#pragma once
#include <library/components/SceneComponent.h>
#include <library/DirectXForwardDeclarations.h>

namespace library
{
	class Mesh;
	class KeyboardComponent;
	class MouseComponent;
} // namespace library

namespace demo
{
	class TextureModelComponent : public library::SceneComponent
	{
		RTTI_CLASS(TextureModelComponent, library::SceneComponent)

	public:
		explicit TextureModelComponent(
			const library::Application& app,
			const library::CameraComponent& camera,
			const library::KeyboardComponent& keyboard,
			const library::MouseComponent& mouse
		);

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		void CreateVertexBuffer(const ComPtr<ID3D11Device>& device, const library::Mesh& mesh);

		ComPtr<ID3DX11Effect> m_effect;
		ComPtr<ID3DX11EffectTechnique> m_technique;
		ComPtr<ID3DX11EffectPass> m_pass;
		ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;

		// texture mapping specific
		ComPtr<ID3D11ShaderResourceView> m_textureShaderResourceView;
		ComPtr<ID3DX11EffectShaderResourceVariable> m_colorTextureVariable;

		unsigned m_indicesCount;

		long m_wheel;

		std::reference_wrapper<const library::KeyboardComponent> m_keyboard;
		std::reference_wrapper<const library::MouseComponent> m_mouse;
	};

} // namespace demo
