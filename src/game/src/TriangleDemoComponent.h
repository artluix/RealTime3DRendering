#pragma once
#include "library/components/SceneComponent.h"
#include "library/Math.h"

class TriangleDemoComponent : public library::rtti::Class<TriangleDemoComponent, library::components::SceneComponent>
{
public:
	explicit TriangleDemoComponent(const library::Application& app, const library::components::CameraComponent& camera);

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

