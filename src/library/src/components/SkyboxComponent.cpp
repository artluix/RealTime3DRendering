#include "StdAfx.h"
#include "library/Components/SkyboxComponent.h"

#include "library/Components/CameraComponent.h"

#include "library/Application.h"

#include "library/Model/Model.h"
#include "library/Effect/EffectVariable.h"

namespace library
{
SkyboxComponent::SkyboxComponent(const std::string& cubeMapName, const float scale)
	: m_cubeMapName(cubeMapName)
{
	SetScaling(math::Vector3(scale));
}

//-------------------------------------------------------------------------

void SkyboxComponent::InitializeInternal()
{
	InitializeMaterial("Skybox");

	Model model(GetApp(), "Sphere", true);
	m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);

	m_textures.resize(Texture::Count);
	m_textures[Texture::Skybox] = GetApp().LoadTexture(m_cubeMapName);
}

void SkyboxComponent::Update(const Time& time)
{
	if (auto camera = GetCamera())
	{
		const auto& position = camera->GetPosition();
		SetPosition(position);
	}

	ConcreteMaterialPrimitiveComponent::Update(time);
}

void SkyboxComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_material->GetWorldViewProjection() << wvp;
	m_material->GetSkyboxTexture() << GetTexture(Texture::Skybox);

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
} // namespace library
