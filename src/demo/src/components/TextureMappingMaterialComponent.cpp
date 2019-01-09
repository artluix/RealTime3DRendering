#include "components/TextureMappingMaterialComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Exception.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectTechnique.h>
#include <library/effect/EffectVariable.h>
#include <library/effect/EffectFactory.h>

#include <DDSTextureLoader.h>

namespace demo
{
	using namespace library;

	namespace
	{
		constexpr float k_rotationAngle = math::constants::Pi_Div_2;
		constexpr float k_movementRate = 0.01f;

		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			fs::Path("../data/effects/TextureMapping_d.fxc")
#else
			fs::Path("../data/effects/TextureMapping.fxc")
#endif
		);
		const auto k_modelPath = utils::GetExecutableDirectory().Join(fs::Path("../data/models/Sphere.obj"));
		const auto k_texturePath = utils::GetExecutableDirectory().Join(fs::Path("../data/textures/EarthComposite.dds"));
	}

	TextureMappingMaterialComponent::TextureMappingMaterialComponent(const Application& app)
		: SceneComponent()
		, ConcreteMaterialComponent(app, k_modelPath)
		, InputReceivableComponent()
	{
	}

	void TextureMappingMaterialComponent::Initialize()
	{
		m_effect = library::EffectFactory::Create(m_app, k_effectPath);
		m_effect->LoadCompiled();

		m_material = std::make_unique<Material>(*m_effect);
		m_material->Initialize();

		MaterialComponent::Initialize();

		// Load the texture
		{
			std::vector<library::byte> textureData;
			utils::LoadBinaryFile(k_texturePath, textureData);
			if (textureData.empty())
			{
				throw Exception("Load texture failed.");
			}

			auto hr = DirectX::CreateDDSTextureFromMemory(
				m_app.GetD3DDevice(),
				reinterpret_cast<const std::uint8_t*>(textureData.data()),
				textureData.size(),
				nullptr,
				m_textureShaderResourceView.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("CreateDDSTextureFromMemory() failed.", hr);
			}
		}
	}

	void TextureMappingMaterialComponent::Update(const Time& time)
	{
		if (!!m_keyboard)
		{
			// rotation
			if (m_keyboard->IsKeyDown(Key::R))
			{
				const auto rotationDelta = k_rotationAngle * time.elapsed.GetSeconds<float>();
				Rotate(rotationDelta);
			}

			// movement
			{
				math::Vector2 movementAmount;
				if (m_keyboard->IsKeyDown(Key::Up))
				{
					movementAmount.y += 1.0f;
				}

				if (m_keyboard->IsKeyDown(Key::Down))
				{
					movementAmount.y -= 1.0f;
				}

				if (m_keyboard->IsKeyDown(Key::Left))
				{
					movementAmount.x -= 1.0f;
				}

				if (m_keyboard->IsKeyDown(Key::Right))
				{
					movementAmount.x += 1.0f;
				}

				if (movementAmount)
				{
					Translate(math::Vector3(movementAmount * k_movementRate));
				}
			}
		}
	}

	void TextureMappingMaterialComponent::SetEffectData()
	{
		auto wvp = m_worldMatrix;
		if (!!m_camera)
			wvp *= m_camera->GetViewProjectionMatrix();
		m_material->GetWVP() << wvp;

		m_material->GetColorTexture() << m_textureShaderResourceView.Get();

		MaterialComponent::SetEffectData();
	}

} // namespace demo
