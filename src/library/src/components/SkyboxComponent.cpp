#include "StdAfx.h"
#include "library/components/SkyboxComponent.h"

#include "library/components/CameraComponent.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectPass.h"
#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectVariable.h"
#include "library/materials/SkyboxMaterial.h"

#include <DDSTextureLoader.h>

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
		MaterialComponent::Initialize(app);
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

	void SkyboxComponent::SetEffectData()
	{
		auto wvp = GetWorldMatrix();
		if (auto camera = GetCamera())
			wvp *= camera->GetViewProjectionMatrix();

		m_material->GetWorldViewProjection() << wvp;
		m_material->GetSkyboxTexture() << m_textureShaderResourceView.Get();

		MaterialComponent::SetEffectData();
	}
} // namespace library
