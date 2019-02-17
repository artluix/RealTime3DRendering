#include "StdAfx.h"
#include "library/components/SkyboxComponent.h"

#include "library/components/CameraComponent.h"

#include "library/Application.h"

#include "library/effect/EffectVariable.h"

namespace library
{
	SkyboxComponent::SkyboxComponent(const std::string& cubeMapName, const float scale)
	{
		SetDefaultModelName("Sphere");
		SetDefaultTextureName(cubeMapName);
		SetScaling(math::Vector3(scale));
	}

	void SkyboxComponent::Initialize(const Application& app)
	{
		InitializeMaterial(app, "Skybox");
		DrawableInputMaterialComponent::Initialize(app);
	}

	void SkyboxComponent::Update(const Time& time)
	{
		if (auto camera = GetCamera())
		{
			const auto& position = camera->GetPosition();
			SetPosition(position);
		}

		AddToRenderer();
	}

	void SkyboxComponent::Draw_SetData()
	{
		auto wvp = GetWorldMatrix();
		if (auto camera = GetCamera())
			wvp *= camera->GetViewProjectionMatrix();

		m_material->GetWorldViewProjection() << wvp;
		m_material->GetSkyboxTexture() << m_defaultTexture.Get();

		DrawableInputMaterialComponent::Draw_SetData();
	}
} // namespace library
