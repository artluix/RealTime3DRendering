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
	namespace
	{
		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			Path("../data/effects/Skybox_d.fxc")
#else
			Path("../data/effects/Skybox.fxc")
#endif
		);
		const auto k_modelPath = utils::GetExecutableDirectory().Join(Path("../data/models/Sphere.obj"));
	}

	SkyboxComponent::SkyboxComponent(const Application& app, const Path& cubeMapPath, const float scale)
		: SceneComponent()
		, MaterialComponentGlue(app)
	{
		SetModelPath(k_modelPath);
		SetTexturePath(cubeMapPath);
		SetScaling(math::Vector3(scale));
	}

	void SkyboxComponent::Initialize()
	{
		m_effect = Effect::Create(m_app, k_effectPath);
		m_effect->LoadCompiled();

		m_material = std::make_unique<SkyboxMaterial>(*m_effect);
		m_material->Initialize();

		MaterialComponent::Initialize();
	}

	void SkyboxComponent::Update(const Time& time)
	{
		if (auto camera = GetCamera())
		{
			const auto& position = camera->GetPosition();
			SetPosition(position);
		}
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
