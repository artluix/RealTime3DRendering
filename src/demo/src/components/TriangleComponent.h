#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/DrawableComponent.h>

#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

namespace demo
{
	class TriangleComponent
		: public library::SceneComponent
		, public library::DrawableComponent
	{
		RTTI_CLASS(TriangleComponent, library::SceneComponent, library::DrawableComponent)

	public:
		explicit TriangleComponent();

		void Initialize(const library::Application& app) override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		ComPtr<ID3DX11Effect> m_effect;
		ComPtr<ID3DX11EffectTechnique> m_technique;
		ComPtr<ID3DX11EffectPass> m_pass;
		ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11Buffer> m_vertexBuffer;
	};
} // namespace demo
