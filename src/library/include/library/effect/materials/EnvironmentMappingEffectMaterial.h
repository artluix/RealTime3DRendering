#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class EnvironmentMappingEffectMaterial : public EffectMaterial
	{
		RTTI_CLASS(EnvironmentMappingEffectMaterial, EffectMaterial)

	public:
		using Vertex = VertexPositionTextureNormal;

		explicit EnvironmentMappingEffectMaterial(const Effect& effect);
		~EnvironmentMappingEffectMaterial();

		EffectVariable& GetAmbientColor() const { return m_ambientColor; }
		EffectVariable& GetEnvironmentColor() const { return m_environmentColor; }
		EffectVariable& GetCameraPosition() const { return m_cameraPosition; }

		EffectVariable& GetWVP() const { return m_wvp; }
		EffectVariable& GetWorld() const { return m_world; }
		EffectVariable& GetReflectionAmount() const { return m_reflectionAmount; }

		EffectVariable& GetColorTexture() const { return m_colorTexture; }
		EffectVariable& GetEnvironmentMap() const { return m_environmentMap; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_ambientColor;
		EffectVariable& m_environmentColor;
		EffectVariable& m_cameraPosition;

		EffectVariable& m_wvp;
		EffectVariable& m_world;
		EffectVariable& m_reflectionAmount;

		EffectVariable& m_colorTexture;
		EffectVariable& m_environmentMap;
	};
} // namespace library
