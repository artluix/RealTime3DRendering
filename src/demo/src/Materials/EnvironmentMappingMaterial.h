#pragma once
#include <library/Materials/Material.h>

class EnvironmentMappingMaterial : public library::Material
{
	RTTI_CLASS(EnvironmentMappingMaterial, library::Material)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit EnvironmentMappingMaterial(library::Effect& effect);
	~EnvironmentMappingMaterial();

	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }
	library::EffectVariable& GetEnvironmentColor() { return m_environmentColor; }
	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }
	library::EffectVariable& GetReflectionAmount() { return m_reflectionAmount; }

	library::EffectVariable& GetColorTexture() { return m_colorTexture; }
	library::EffectVariable& GetEnvironmentMap() { return m_environmentMap; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_ambientColor;
	library::EffectVariable& m_environmentColor;
	library::EffectVariable& m_cameraPosition;

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_reflectionAmount;

	library::EffectVariable& m_colorTexture;
	library::EffectVariable& m_environmentMap;
};
