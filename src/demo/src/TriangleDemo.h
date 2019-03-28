#pragma once
#include <library/Components/SceneComponent.h>

#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

class TriangleDemo : public library::SceneComponent
{
	RTTI_CLASS(TriangleDemo, library::SceneComponent)

public:
	explicit TriangleDemo() = default;

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

	unsigned GetVertexSize() const override;

protected:
	void Draw_SetData(const library::MeshData& meshData) override;

private:
	ComPtr<ID3DX11Effect> m_effect;
	ComPtr<ID3DX11EffectTechnique> m_technique;
	ComPtr<ID3DX11EffectPass> m_pass;
	ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
};
