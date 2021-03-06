#pragma once
#include <library/Materials/Material.h>

class PointSpriteMaterial : public library::Material
{
	RTTI_CLASS(PointSpriteMaterial, library::Material)

public:
	using Vertex = library::VertexPositionSize;

	explicit PointSpriteMaterial(library::Effect& effect);

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	library::EffectVariable& GetCameraUp() { return m_cameraUp; }

	library::EffectVariable& GetViewProjection() { return m_viewProjection; }

	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_cameraUp;

	library::EffectVariable& m_viewProjection;

	library::EffectVariable& m_colorTexture;
};
