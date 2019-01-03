#include "StdAfx.h"
#include "library/components/SkyboxComponent.h"

#include "library/components/CameraComponent.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"

#include "library/effects/Effect.h"
#include "library/effects/SkyboxMaterial.h"
#include "library/effects/EffectPass.h"
#include "library/effects/EffectTechnique.h"
#include "library/effects/EffectVariable.h"

#include "library/Model.h"
#include "library/Mesh.h"

#include <DDSTextureLoader.h>

namespace library
{
	namespace
	{
		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			fs::Path("../data/effects/Skybox_d.fxc")
#else
			fs::Path("../data/effects/Skybox.fxc")
#endif
		);
		const auto k_modelPath = utils::GetExecutableDirectory().Join(
			fs::Path("../data/models/Sphere.obj")
		);
	}

	SkyboxComponent::SkyboxComponent(
		const Application& app,
		const CameraComponent& camera,
		const fs::Path& cubeMapPath,
		const float scale
	)
		: Class(app, camera)
		, m_cubeMapPath(cubeMapPath)
	{
		SetScaling(math::Vector3(scale));
	}

	void SkyboxComponent::Initialize()
	{
		Model model(m_app, k_modelPath, true);

		// create effect
		m_effect = std::make_unique<library::Effect>(m_app);
		m_effect->LoadCompiledEffect(k_effectPath);

		// create material
		m_material = std::make_unique<SkyboxMaterial>(*m_effect);
		m_material->Initialize();

		const auto& mesh = model.GetMesh(0);
		m_vertexBuffer = m_material->CreateVertexBuffer(m_app.GetD3DDevice(), mesh);
		m_indexBuffer = mesh.CreateIndexBuffer();
		m_indicesCount = mesh.GetIndicesCount();

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
		const CameraComponent& camera = m_camera.get();
		const auto& position = camera.GetPosition();

		SetPosition(position);
	}

	void SkyboxComponent::Draw(const Time& time)
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
		m_material->GetSkyboxTexture() << m_cubeMapShaderResourceView.Get();

		pass.Apply(0, d3dDeviceContext);

		d3dDeviceContext->DrawIndexed(m_indicesCount, 0, 0);
	}

} // namespace library
