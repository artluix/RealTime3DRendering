#pragma once
#include "Materials/PhongLightingMaterial.h"

class FogMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(FogMaterial, PhongLightingMaterial)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit FogMaterial(library::Effect& effect);
	~FogMaterial();

	library::EffectVariable& GetFogColor() { return m_fogColor; }
	library::EffectVariable& GetFogStart() { return m_fogStart; }
	library::EffectVariable& GetFogRange() { return m_fogRange; }

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }
	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

protected:
	explicit FogMaterial(library::Effect& effect, const std::string& defaultTechniqueName);

	void InitializeInternal() override;

private:
	library::EffectVariable& m_fogColor;
	library::EffectVariable& m_fogStart;
	library::EffectVariable& m_fogRange;

	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_colorTexture;
};
