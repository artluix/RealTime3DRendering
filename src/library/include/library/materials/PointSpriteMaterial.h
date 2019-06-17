#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
class PointSpriteMaterial : public Material
{
	RTTI_CLASS(PointSpriteMaterial, Material)

public:
	using Vertex = VertexPositionSize;

	explicit PointSpriteMaterial(Effect& effect);

	EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	EffectVariable& GetCameraUp() { return m_cameraUp; }

	EffectVariable& GetViewProjection() { return m_viewProjection; }

	EffectVariable& GetColorTexture() { return m_colorTexture; }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;

	unsigned GetVertexSize() const override { return sizeof(Vertex); }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_cameraPosition;
	EffectVariable& m_cameraUp;

	EffectVariable& m_viewProjection;

	EffectVariable& m_colorTexture;
};
} // namespace library
