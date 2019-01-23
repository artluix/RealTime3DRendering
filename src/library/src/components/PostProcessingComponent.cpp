#include "StdAfx.h"
#include "library/components/PostProcessingComponent.h"

#include "library/components/FullScreenQuadComponent.h"

namespace library
{
	PostProcessingComponent::~PostProcessingComponent() = default;

	//-------------------------------------------------------------------------

	void PostProcessingComponent::SetSceneTexture(ID3D11ShaderResourceView& sceneTexture)
	{
		if (m_sceneTexture != &sceneTexture)
		{
			m_sceneTexture = &sceneTexture;
		}
	}

	void PostProcessingComponent::InitializeQuad(const std::string& techniqueName, const std::string& passName /*= "p0"*/)
	{
		m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>(m_app);
		m_fullScreenQuad->SetMaterial(GetMaterial(), techniqueName, passName);
		m_fullScreenQuad->Initialize();
	}
} // namespace library
