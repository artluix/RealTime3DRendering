#pragma once
#include "VertexInstance.h"
#include "Materials/LightsMaterial.h"

class InstancingMaterial : public LightsMaterial
{
	RTTI_CLASS(InstancingMaterial, LightsMaterial)

public:
	using Vertex = library::VertexPositionTextureNormal;
	using VertexInstance = library::VertexInstanceSpecular;

	explicit InstancingMaterial(library::Effect& effect);
	~InstancingMaterial();

	library::EffectVariable& GetViewProjection() { return m_viewProjection; }

	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }
	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_viewProjection;

	library::EffectVariable& m_ambientColor;

	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_colorTexture;
};
