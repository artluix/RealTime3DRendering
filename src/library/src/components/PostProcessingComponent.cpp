#include "StdAfx.h"
#include "library/Components/PostProcessingComponent.h"

#include "library/Components/FullScreenQuadComponent.h"

namespace library
{
PostProcessingComponent::~PostProcessingComponent() = default;

//-------------------------------------------------------------------------

void PostProcessingComponent::SetSceneTexture(ID3D11ShaderResourceView& sceneTexture)
{
	m_sceneTexture = &sceneTexture;
}

//-------------------------------------------------------------------------

void PostProcessingComponent::Update(const Time& time)
{}

void PostProcessingComponent::InitializeQuad()
{
	m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>();

	if (auto material = GetMaterial())
	{
		m_fullScreenQuad->SetMaterial(*material);
	}

	m_fullScreenQuad->Initialize(GetApp());
}

void PostProcessingComponent::InitializeQuad(
	const std::string& techniqueName,
	const std::string& passName /*= "p0"*/
)
{
	m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>();

	if (auto material = GetMaterial())
	{
		m_fullScreenQuad->SetMaterial(*material, techniqueName, passName);
	}

	m_fullScreenQuad->Initialize(GetApp());
}
} // namespace library
