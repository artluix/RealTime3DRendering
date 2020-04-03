#pragma once
#include "Materials/PhongLightingMaterial.h"

class DisplacementMappingMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(DisplacementMappingMaterial, PhongLightingMaterial)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit DisplacementMappingMaterial(library::Effect& effect);
	~DisplacementMappingMaterial();

	library::EffectVariable& GetDisplacementScale() { return m_displacementScale; }
	library::EffectVariable& GetDisplacementMap() { return m_displacementMap; }

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }
	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_displacementScale;
	library::EffectVariable& m_displacementMap;

	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_colorTexture;
};
