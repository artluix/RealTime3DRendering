#include "components/TextureMappingMaterialComponent.h"

#include "materials/TextureMappingMaterial.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Exception.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectPass.h>
#include <library/effect/EffectTechnique.h>
#include <library/effect/EffectVariable.h>

#include <library/Model.h>
#include <library/Mesh.h>

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
		const auto k_modelPath = utils::GetExecutableDirectory().Join(
			fs::Path("../data/models/Sphere.obj")
		);
		const auto k_texturePath = utils::GetExecutableDirectory().Join(
			fs::Path("../data/textures/EarthComposite.dds")
		);
	}

	TextureMappingMaterialComponent::TextureMappingMaterialComponent(
		const Application& app,
		const CameraComponent& camera,
		const KeyboardComponent& keyboard
	)
		: Class(app, camera)
		, m_keyboard(keyboard)
	{
	}

	void TextureMappingMaterialComponent::Initialize()
	{
		Model model(m_app, k_modelPath, true);

		// create effect
		m_effect = std::make_unique<library::Effect>(m_app);
		m_effect->LoadCompiledEffect(k_effectPath);

		// create material
		m_material = std::make_unique<TextureMappingMaterial>(*m_effect);
		m_material->Initialize();

		const auto& mesh = model.GetMesh(0);
		m_vertexBuffer = m_material->CreateVertexBuffer(m_app.GetD3DDevice(), mesh);
		m_indexBuffer = mesh.CreateIndexBuffer();
		m_indicesCount = mesh.GetIndicesCount();

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
		const KeyboardComponent& keyboard = m_keyboard;

		// rotation
		if (keyboard.IsKeyDown(Key::R))
		{
			const auto rotationDelta = k_rotationAngle * time.elapsed.GetSeconds<float>();
			Rotate(rotationDelta);
		}

		// movement
		{
			math::Vector2 movementAmount;
			if (keyboard.IsKeyDown(Key::Up))
			{
				movementAmount.y += 1.0f;
			}

			if (keyboard.IsKeyDown(Key::Down))
			{
				movementAmount.y -= 1.0f;
			}

			if (keyboard.IsKeyDown(Key::Left))
			{
				movementAmount.x -= 1.0f;
			}

			if (keyboard.IsKeyDown(Key::Right))
			{
				movementAmount.x += 1.0f;
			}

			if (movementAmount)
			{
				Translate(math::Vector3(movementAmount * k_movementRate));
			}
		}
	}

	void TextureMappingMaterialComponent::Draw(const Time& time)
	{
		auto d3dDeviceContext = m_app.GetD3DDeviceContext();
		d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// input layout
		auto& pass = m_material->GetCurrentTechnique().GetPass(0);
		auto inputLayout = m_material->GetInputLayout(pass);
		d3dDeviceContext->IASetInputLayout(inputLayout);

		// vertex & index buffers
		unsigned stride = m_material->GetVertexSize();
		unsigned offset = 0;
		d3dDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		d3dDeviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		const auto wvp = m_worldMatrix * GetCamera().GetViewProjectionMatrix();
		m_material->GetWorldViewProjection() << wvp;
		m_material->GetColorTexture() << m_textureShaderResourceView.Get();

		pass.Apply(0, d3dDeviceContext);

		d3dDeviceContext->DrawIndexed(m_indicesCount, 0, 0);
	}

} // namespace demo
