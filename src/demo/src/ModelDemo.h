#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/InputReceivableComponent.h>

#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

namespace library
{
	class Mesh;
	class KeyboardComponent;
} // namespace library

class ModelDemo
	: public library::SceneComponent
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ModelDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit ModelDemo() = default;

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData() override;

	unsigned GetVertexSize() const override;

private:
	void CreateVertexBuffer(const ComPtr<ID3D11Device>& device, const library::Mesh& mesh);

	ComPtr<ID3DX11Effect> m_effect;
	ComPtr<ID3DX11EffectTechnique> m_technique;
	ComPtr<ID3DX11EffectPass> m_pass;
	ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
};
