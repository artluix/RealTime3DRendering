#include "StdAfx.h"
#include "library/model/Mesh.h"

#include "library/model/Model.h"
#include "library/Application.h"
#include "library/Exception.h"

#include <assimp/scene.h>

#include <cassert>

namespace library
{
	Mesh::Mesh(Model& model, aiMesh& mesh)
		: m_model(model)
		, m_material(m_model.GetMaterial(mesh.mMaterialIndex))
		, m_name(mesh.mName.C_Str())
		, m_facesCount(0)
	{
		// Vertices
		{
			m_vertices.reserve(mesh.mNumVertices);

			for (unsigned i = 0; i < mesh.mNumVertices; i++)
			{
				m_vertices.push_back(reinterpret_cast<const DirectX::XMFLOAT3&>(mesh.mVertices[i]));
			}
		}

		// Normals
		if (mesh.HasNormals())
		{
			m_normals.reserve(mesh.mNumVertices);

			for (unsigned i = 0; i < mesh.mNumVertices; i++)
			{
				m_normals.push_back(reinterpret_cast<const DirectX::XMFLOAT3&>(mesh.mNormals[i]));
			}
		}

		// Tangents and Binormals
		if (mesh.HasTangentsAndBitangents())
		{
			m_tangentBinormals.reserve(mesh.mNumVertices);

			for (unsigned i = 0; i < mesh.mNumVertices; i++)
			{
				const auto& tangent = reinterpret_cast<const DirectX::XMFLOAT3&>(mesh.mTangents[i]);
				const auto& binormal = reinterpret_cast<const DirectX::XMFLOAT3&>(mesh.mBitangents[i]);
				m_tangentBinormals.emplace_back(tangent, binormal);
			}
		}

		// Textures coordinates
		{
			const unsigned uvChannelsCount = mesh.GetNumUVChannels();
			m_texturesCoordinates.reserve(uvChannelsCount);

			for (unsigned i = 0; i < uvChannelsCount; i++)
			{
				DirectX::XMFLOAT3Vector textureCoordinates;
				textureCoordinates.reserve(mesh.mNumVertices);

				const auto aiTextureCoords = mesh.mTextureCoords[i];

				for (unsigned j = 0; j < mesh.mNumVertices; j++)
				{
					textureCoordinates.push_back(reinterpret_cast<const DirectX::XMFLOAT3&>(aiTextureCoords[j]));
				}

				m_texturesCoordinates.push_back(textureCoordinates);
			}
		}

		// Vertices Colors
		{
			const unsigned colorChannelsCount = mesh.GetNumColorChannels();
			m_verticesColors.reserve(colorChannelsCount);

			for (unsigned i = 0; i < colorChannelsCount; i++)
			{
				DirectX::XMFLOAT4Vector vertexColors;
				vertexColors.reserve(mesh.mNumVertices);

				const auto aiVertexColors = mesh.mTextureCoords[i];

				for (unsigned j = 0; j < mesh.mNumVertices; j++)
				{
					vertexColors.push_back(reinterpret_cast<const DirectX::XMFLOAT4&>(aiVertexColors[j]));
				}

				m_verticesColors.push_back(vertexColors);
			}
		}

		// Faces (could pre-reserve if we limit primitive types)
		if (mesh.HasFaces())
		{
			m_facesCount = mesh.mNumFaces;
			for (unsigned i = 0; i < m_facesCount; i++)
			{
				const auto& face = mesh.mFaces[i];

				for (unsigned j = 0; j < face.mNumIndices; j++)
				{
					m_indices.emplace_back(face.mIndices[j]);
				}
			}
		}
	}

	Mesh::~Mesh() = default;

	const DirectX::XMFLOAT3Vector& Mesh::GetTextureCoordinates(const unsigned textureIdx) const
	{
		return m_texturesCoordinates[textureIdx];
	}

	const DirectX::XMFLOAT4Vector& Mesh::GetVertexColors(const unsigned vertexIdx) const
	{
		return m_verticesColors[vertexIdx];
	}

	ComPtr<ID3D11Buffer> Mesh::CreateIndexBuffer() const
	{
		D3D11_BUFFER_DESC indexBufferDesc{};
		indexBufferDesc.ByteWidth = sizeof(unsigned) * m_indices.size();
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{};
		indexSubResourceData.pSysMem = m_indices.data();

		ComPtr<ID3D11Buffer> indexBuffer;
		auto hr = m_model.GetApplication().GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexSubResourceData,
			indexBuffer.GetAddressOf()
		);

		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.");
		}

		return indexBuffer;
	}

} // namespace library
