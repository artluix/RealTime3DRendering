#pragma once
#include <library/components/SceneComponent.h>

struct ID3D11Buffer;
struct ID3D11Device;

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectPass;
struct ID3DX11EffectMatrixVariable;

namespace demo
{
	class TriangleComponent : public library::rtti::Class<TriangleComponent, library::components::SceneComponent>
	{
	public:
		explicit TriangleComponent(const library::Application& app, const library::components::CameraComponent& camera);

		void Initialize() override;
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
