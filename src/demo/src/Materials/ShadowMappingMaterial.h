#pragma once
#include "Materials/PhongLightingMaterial.h"
#include "Materials/DepthMapMaterial.h"

class ShadowMappingMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(ShadowMappingMaterial, PhongLightingMaterial)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit ShadowMappingMaterial(library::Effect& effect);

	library::EffectVariable& GetProjectiveTextureMatrix() { return m_projectiveTextureMatrix; }
	library::EffectVariable& GetShadowMapTexture() { return m_shadowMapTexture; }
	library::EffectVariable& GetShadowMapSize() { return m_shadowMapSize; }

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }
	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_projectiveTextureMatrix;
	library::EffectVariable& m_shadowMapTexture;
	library::EffectVariable& m_shadowMapSize;

	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_colorTexture;
};

//-------------------------------------------------------------------------

struct ShadowMappingTechnique
{
	enum Type : unsigned
	{
		Simple = 0,
		ManualPCF,
		PCF,

		//# Count
		Count
	};

	static constexpr DepthMappingTechnique::Type GetDepthMapTechniqueType(const Type t)
	{
		switch (t)
		{
			case PCF: return DepthMappingTechnique::Bias;
			default: return DepthMappingTechnique::Create;
		}
	}

	static constexpr Type Next(const Type t) { return static_cast<Type>((t + 1) % Count); }

	static constexpr char* ToDisplayString(const Type t)
	{
		switch (t)
		{
			case Simple: return "Simple";
			case ManualPCF: return "Manual PCF";
			case PCF: return "PCF";
			default: return "";
		};
	}

	static constexpr char* ToString(const Type t)
	{
		switch (t)
		{
			case Simple: return "shadow_mapping";
			case ManualPCF: return "shadow_mapping_manual_pcf";
			case PCF: return "shadow_mapping_pcf";
			default: return "";
		};
	}
};
