#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/DrawableComponent.h>
#include <library/components/InputReceivableComponent.h>
#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

namespace library
{
	class Mesh;
	class KeyboardComponent;
} // namespace library

namespace demo
{
	class ModelComponent
		: public library::SceneComponent
		, public library::DrawableComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(ModelComponent, library::SceneComponent, library::DrawableComponent, library::InputReceivableComponent)

	public:
		explicit ModelComponent();

		void Initialize(const library::Application& app) override;
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

		unsigned m_indicesCount;
	};
} // namespace demo
