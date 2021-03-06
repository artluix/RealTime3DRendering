#pragma once
#include "library/Materials/Material.h"

namespace library
{
class PostProcessingMaterial : public Material
{
	RTTI_CLASS(PostProcessingMaterial, Material)

public:
	using Vertex = VertexPositionTexture;

	explicit PostProcessingMaterial(Effect& effect);
	~PostProcessingMaterial();

	EffectVariable& GetSceneTexture() { return m_sceneTexture; }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_sceneTexture;
};
} // namespace library
