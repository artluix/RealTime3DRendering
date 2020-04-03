#pragma once
#include "Materials/TessellationMaterial.h"

class ModelTessellationMaterial : public TessellationMaterial
{
	RTTI_CLASS(ModelTessellationMaterial, TessellationMaterial)

public:
	using Vertex = library::VertexPositionTexture;

	explicit ModelTessellationMaterial(library::Effect& effect);

	library::EffectVariable& GetWorld() { return m_world; }
	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

	library::EffectVariable& GetMaxTessFactor() { return m_maxTessFactor; }
	library::EffectVariable& GetMinTessDistance() { return m_minTessDistance; }
	library::EffectVariable& GetMaxTessDistance() { return m_maxTessDistance; }

protected:
	void InitializeInternal() override;

	library::EffectVariable& m_world;
	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_colorTexture;

	library::EffectVariable& m_maxTessFactor;
	library::EffectVariable& m_minTessDistance;
	library::EffectVariable& m_maxTessDistance;
};
