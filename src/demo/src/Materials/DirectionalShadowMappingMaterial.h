#pragma once
#include "Materials/DirectionalLightMaterial.h"
#include "Materials/DepthMapMaterial.h"

class DirectionalShadowMappingMaterial : public DirectionalLightMaterial
{
	RTTI_CLASS(DirectionalShadowMappingMaterial, DirectionalLightMaterial)

public:
	explicit DirectionalShadowMappingMaterial(library::Effect& effect);

	library::EffectVariable& GetProjectiveTextureMatrix() { return m_projectiveTextureMatrix; }
	library::EffectVariable& GetShadowMapTexture() { return m_shadowMapTexture; }
	library::EffectVariable& GetShadowMapSize() { return m_shadowMapSize; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_projectiveTextureMatrix;
	library::EffectVariable& m_shadowMapTexture;
	library::EffectVariable& m_shadowMapSize;
};

//-------------------------------------------------------------------------

struct DirectionalShadowMappingTechnique
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
			case Simple:		return "Simple";
			case ManualPCF:		return "Manual PCF";
			case PCF:			return "PCF";
			default:			return "";
		};
	}

	static constexpr char* ToString(const Type t)
	{
		switch (t)
		{
			case Simple:		return "directional_shadow_mapping";
			case ManualPCF:		return "directional_shadow_mapping_manual_pcf";
			case PCF:			return "directional_shadow_mapping_pcf";
			default:			return "";
		};
	}
};
