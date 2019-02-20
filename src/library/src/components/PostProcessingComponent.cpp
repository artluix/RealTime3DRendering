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

	//-------------------------------------------------------------------------

	void PostProcessingComponent::InitializeQuad(const Application& app)
	{
		auto material = GetMaterial();
		assert(!!material);

		m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>();
		m_fullScreenQuad->SetMaterial(*material);
		m_fullScreenQuad->Initialize(app);
	}

	void PostProcessingComponent::InitializeQuad(
		const Application& app,
		const std::string& techniqueName,
		const std::string& passName /*= "p0"*/
	)
	{
		auto material = GetMaterial();
		assert(!!material);

		m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>();
		m_fullScreenQuad->SetMaterial(*material, techniqueName, passName);
		m_fullScreenQuad->Initialize(app);
	}
} // namespace library
