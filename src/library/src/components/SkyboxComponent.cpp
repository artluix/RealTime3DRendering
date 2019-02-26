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
		SceneComponent::Initialize(app);
	}

	void SkyboxComponent::Update(const Time& time)
	{
		if (!!m_camera)
		{
			const auto& position = m_camera->GetPosition();
			SetPosition(position);
		}

		SceneComponent::Update(time);
	}

	void SkyboxComponent::Draw_SetData()
	{
		auto wvp = GetWorldMatrix();
		if (!!m_camera)
			wvp *= m_camera->GetViewProjectionMatrix();

		m_material->GetWorldViewProjection() << wvp;
		m_material->GetSkyboxTexture() << GetTexture();

		SceneComponent::Draw_SetData();
	}
} // namespace library
