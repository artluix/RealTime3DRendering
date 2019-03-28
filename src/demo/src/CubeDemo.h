#pragma once
#include <library/Components/SceneComponent.h>
#include <library/Components/InputReceivableComponent.h>

#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

namespace library
{
	class KeyboardComponent;
} // namespace library

class CubeDemo
	: public library::SceneComponent
	, public library::InputReceivableComponent
{
	RTTI_CLASS(CubeDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit CubeDemo() = default;

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::MeshData& meshData) override;

	unsigned GetVertexSize() const override;

private:
	ComPtr<ID3DX11Effect> m_effect;
	ComPtr<ID3DX11EffectTechnique> m_technique;
	ComPtr<ID3DX11EffectPass> m_pass;
	ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;
};
