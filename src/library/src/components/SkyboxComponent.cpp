#include "StdAfx.h"
#include "library/components/SkyboxComponent.h"

#include "library/components/CameraComponent.h"

#include "library/Application.h"

#include "library/effect/EffectVariable.h"

namespace library
{
	SkyboxComponent::SkyboxComponent(const std::string& cubeMapName, const float scale)
	{
		SetModelName("Sphere");
		SetTextureName(cubeMapName);
		SetScaling(math::Vector3(scale));
	}

	void SkyboxComponent::Initialize(const Application& app)
	{
		InitializeMaterial(app, "Skybox");
		MaterialSceneComponent::Initialize(app);
	}

	void SkyboxComponent::Update(const Time& time)
	{
		if (auto camera = GetCamera())
		{
			const auto& position = camera->GetPosition();
			SetPosition(position);
		}

		MaterialSceneComponent::Update(time);
	}

	void SkyboxComponent::Draw_SetData()
	{
		auto wvp = GetWorldMatrix();
		if (auto camera = GetCamera())
			wvp *= camera->GetViewProjectionMatrix();

		m_material->GetWorldViewProjection() << wvp;
		m_material->GetSkyboxTexture() << GetTexture();

		MaterialSceneComponent::Draw_SetData();
	}
} // namespace library
