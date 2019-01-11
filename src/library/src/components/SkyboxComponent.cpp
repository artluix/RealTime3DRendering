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
#include "library/effect/EffectFactory.h"

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
		: SceneComponent(app)
		, m_cubeMapPath(cubeMapPath)
	{
		SetModelPath(k_modelPath);
		SetScaling(math::Vector3(scale));
	}

	void SkyboxComponent::Initialize()
	{
		m_effect = EffectFactory::Create(m_app, k_effectPath);
		m_effect->LoadCompiled();

		m_material = std::make_unique<SkyboxEffectMaterial>(*m_effect);
		m_material->Initialize();

		DrawableComponent::Initialize();

		// Load the texture
		{
			std::vector<library::byte> textureData;
			utils::LoadBinaryFile(m_cubeMapPath, textureData);
			if (textureData.empty())
			{
				throw Exception("Load texture failed.");
			}

			auto hr = DirectX::CreateDDSTextureFromMemory(
				m_app.GetD3DDevice(),
				reinterpret_cast<const std::uint8_t*>(textureData.data()),
				textureData.size(),
				nullptr,
				m_cubeMapShaderResourceView.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("CreateDDSTextureFromMemory() failed.", hr);
			}
		}
	}

	void SkyboxComponent::Update(const Time& time)
	{
		if (!!m_camera)
		{
			const auto& position = m_camera->GetPosition();
			SetPosition(position);
		}
	}

	void SkyboxComponent::SetEffectData()
	{
		auto wvp = m_worldMatrix;
		if (!!m_camera)
			wvp *= m_camera->GetViewProjectionMatrix();
		m_material->GetWorldViewProjection() << wvp;
			
		m_material->GetSkyboxTexture() << m_cubeMapShaderResourceView.Get();

		DrawableComponent::SetEffectData();
	}
} // namespace library
