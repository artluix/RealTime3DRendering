#pragma once
#include "library/Materials/PointLightMaterial.h"
#include "library/Materials/DepthMapMaterial.h"

namespace library
{
	class ShadowMappingMaterial : public PointLightMaterial
	{
		RTTI_CLASS(ShadowMappingMaterial, PointLightMaterial)

	public:
		explicit ShadowMappingMaterial(Effect& effect);

		EffectVariable& GetProjectiveTextureMatrix() { return m_projectiveTextureMatrix; }
		EffectVariable& GetShadowMapTexture() { return m_shadowMapTexture; }
		EffectVariable& GetShadowMapSize() { return m_shadowMapSize; }

	protected:
		void InitializeInternal() override;

	private:
		EffectVariable& m_projectiveTextureMatrix;
		EffectVariable& m_shadowMapTexture;
		EffectVariable& m_shadowMapSize;
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
				case PCF:	return DepthMappingTechnique::Bias;
				default:	return DepthMappingTechnique::Create;
			}
		}

		static constexpr Type Next(const Type t)
		{
			return static_cast<Type>((t + 1) % Count);
		}

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
				case Simple:		return "shadow_mapping";
				case ManualPCF:		return "shadow_mapping_manual_pcf";
				case PCF:			return "shadow_mapping_pcf";
				default:			return "";
			};
		}
	};
} // namespace library
