#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/InputReceivableComponent.h>
#include <library/DirectXForwardDeclarations.h>

namespace library
{
	class KeyboardComponent;
} // namespace library

namespace demo
{
	class CubeComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(CubeComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit CubeComponent(const library::Application& app);

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		ComPtr<ID3DX11Effect> m_effect;
		ComPtr<ID3DX11EffectTechnique> m_technique;
		ComPtr<ID3DX11EffectPass> m_pass;
		ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;
	};
} // namespace demo
