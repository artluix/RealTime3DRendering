#pragma once
#include "Materials/PbrLightingMaterial.h"

class PbrLightsMaterial : public PbrLightingMaterial
{
	RTTI_CLASS(PbrLightsMaterial, PbrLightingMaterial)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit PbrLightsMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "main11");

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }

	library::EffectVariable& GetAlbedo() { return m_albedo; }
	library::EffectVariable& GetMetallic() { return m_metallic; }
	library::EffectVariable& GetRoughness() { return m_roughness; }
	library::EffectVariable& GetAO() { return m_ao; }

	library::EffectVariable& GetAlbedoMap() { return m_albedoMap; }
	library::EffectVariable& GetMetallicMap() { return m_metallicMap; }
	library::EffectVariable& GetRoughnessMap() { return m_roughnessMap; }
	library::EffectVariable& GetAOMap() { return m_aoMap; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_cameraPosition;

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;

	library::EffectVariable& m_albedo;
	library::EffectVariable& m_metallic;
	library::EffectVariable& m_roughness;
	library::EffectVariable& m_ao;

	library::EffectVariable& m_albedoMap;
	library::EffectVariable& m_metallicMap;
	library::EffectVariable& m_roughnessMap;
	library::EffectVariable& m_aoMap;
};
