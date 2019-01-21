#include "StdAfx.h"
#include "library/components/MaterialComponent.h"

#include "library/effect/EffectPass.h"
#include "library/effect/EffectTechnique.h"

#include "library/materials/Material.h"

#include "library/Model.h"
#include "library/Mesh.h"

#include <DDSTextureLoader.h>

namespace library
{
	void MaterialComponent::LoadModel(const Path& modelPath, ComPtr<ID3D11Buffer>& vertexBuffer, ComPtr<ID3D11Buffer>& indexBuffer)
	{
		if (const auto material = GetMaterial())
		{
			assert(material->IsInitialized());

			if (modelPath)
			{
				Model model(m_app, modelPath, true);

				const auto& mesh = model.GetMesh(0);
				m_vertexBuffer = material->CreateVertexBuffer(m_app.GetDevice(), mesh);
				m_indexBuffer = mesh.CreateIndexBuffer();

				m_indicesCount = mesh.GetIndicesCount();
				m_verticesCount = mesh.GetVerticesCount();
			}
		}
	}

	void MaterialComponent::LoadTexture(const Path& texturePath, ComPtr<ID3D11ShaderResourceView>& textureShaderResourceView)
	{
		if (texturePath)
		{
			assert(texturePath.GetExt().GetString() == "dds");

			std::vector<library::byte> textureData;
			utils::LoadBinaryFile(texturePath, textureData);
			if (textureData.empty())
			{
				throw Exception("Load texture failed.");
			}

			auto hr = DirectX::CreateDDSTextureFromMemory(
				m_app.GetDevice(),
				reinterpret_cast<const std::uint8_t*>(textureData.data()),
				textureData.size(),
				nullptr,
				textureShaderResourceView.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("CreateDDSTextureFromMemory() failed.", hr);
			}
		}
	}

	void MaterialComponent::SetModelPath(const Path& path)
	{
		if (m_modelPath != path)
			m_modelPath = path;
	}

	void MaterialComponent::SetTexturePath(const Path& path)
	{
		if (m_texturePath != path)
			m_texturePath = path;
	}
}
