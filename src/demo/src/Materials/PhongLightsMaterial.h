#pragma once
#include "Materials/PhongLightingMaterial.h"

class PhongLightsMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(PhongLightsMaterial, PhongLightingMaterial)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit PhongLightsMaterial(library::Effect& effect);

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }

	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_cameraPosition;

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;

	library::EffectVariable& m_colorTexture;
};
