#include "StdAfx.h"
#include "library/components/MaterialPostProcessingComponent.h"

namespace library
{
	MaterialPostProcessingComponent::~MaterialPostProcessingComponent() = default;

	//-------------------------------------------------------------------------

	void MaterialPostProcessingComponent::SetSceneTexture(ID3D11ShaderResourceView& sceneTexture)
	{
		if (m_sceneTexture != &sceneTexture)
		{
			m_sceneTexture = &sceneTexture;
		}
	}

	//-------------------------------------------------------------------------

	void MaterialPostProcessingComponent::InitializeQuad(const Application& app)
	{
		m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>();
		m_fullScreenQuad->SetMaterial(GetMaterial());
		m_fullScreenQuad->Initialize(app);
	}

	void MaterialPostProcessingComponent::InitializeQuad(
		const Application& app,
		const std::string& techniqueName,
		const std::string& passName /*= "p0"*/
	)
	{
		m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>();
		m_fullScreenQuad->SetMaterial(GetMaterial(), techniqueName, passName);
		m_fullScreenQuad->Initialize(app);
	}
} // namespace library
