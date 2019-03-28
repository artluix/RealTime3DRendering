#include "StdAfx.h"
#include "library/Model/Mesh.h"

#include "library/Model/Model.h"
#include "library/Application.h"
#include "library/Exception.h"

#include <assimp/scene.h>

#include <cassert>

namespace library
{
	Mesh::Mesh(Model& model, const aiMesh& aiMesh)
		: m_model(model)
		, m_material(m_model.GetMaterial(aiMesh.mMaterialIndex))
		, m_name(aiMesh.mName.C_Str())
		, m_facesCount(0)
	{
		// Vertices
		{
			m_vertices.reserve(aiMesh.mNumVertices);

			for (unsigned i = 0; i < aiMesh.mNumVertices; i++)
			{
				m_vertices.push_back(reinterpret_cast<const DirectX::XMFLOAT3&>(aiMesh.mVertices[i]));
			}
		}

		// Normals
		if (aiMesh.HasNormals())
		{
			m_normals.reserve(aiMesh.mNumVertices);

			for (unsigned i = 0; i < aiMesh.mNumVertices; i++)
			{
				m_normals.push_back(reinterpret_cast<const DirectX::XMFLOAT3&>(aiMesh.mNormals[i]));
			}
		}

		// Tangents and Binormals
		if (aiMesh.HasTangentsAndBitangents())
		{
			m_tangentBinormals.reserve(aiMesh.mNumVertices);

			for (unsigned i = 0; i < aiMesh.mNumVertices; i++)
			{
				const auto& tangent = reinterpret_cast<const DirectX::XMFLOAT3&>(aiMesh.mTangents[i]);
				const auto& binormal = reinterpret_cast<const DirectX::XMFLOAT3&>(aiMesh.mBitangents[i]);
				m_tangentBinormals.emplace_back(tangent, binormal);
			}
		}

		// Textures coordinates
		{
			const unsigned uvChannelsCount = aiMesh.GetNumUVChannels();
			m_texturesCoordinates.reserve(uvChannelsCount);

			for (unsigned i = 0; i < uvChannelsCount; i++)
			{
				DirectX::XMFLOAT3Vector textureCoordinates;
				textureCoordinates.reserve(aiMesh.mNumVertices);

				const auto aiTextureCoords = aiMesh.mTextureCoords[i];

				for (unsigned j = 0; j < aiMesh.mNumVertices; j++)
				{
					textureCoordinates.push_back(
						reinterpret_cast<const DirectX::XMFLOAT3&>(aiTextureCoords[j])
					);
				}

				m_texturesCoordinates.push_back(textureCoordinates);
			}
		}

		// Vertices Colors
		{
			const unsigned colorChannelsCount = aiMesh.GetNumColorChannels();
			m_verticesColors.reserve(colorChannelsCount);

			for (unsigned i = 0; i < colorChannelsCount; i++)
			{
				DirectX::XMFLOAT4Vector vertexColors;
				vertexColors.reserve(aiMesh.mNumVertices);

				const auto aiVertexColors = aiMesh.mTextureCoords[i];

				for (unsigned j = 0; j < aiMesh.mNumVertices; j++)
				{
					vertexColors.push_back(reinterpret_cast<const DirectX::XMFLOAT4&>(aiVertexColors[j]));
				}

				m_verticesColors.push_back(vertexColors);
			}
		}

		// Faces (could pre-reserve if we limit primitive types)
		if (aiMesh.HasFaces())
		{
			m_facesCount = aiMesh.mNumFaces;
			for (unsigned i = 0; i < m_facesCount; i++)
			{
				const auto& face = aiMesh.mFaces[i];

				for (unsigned j = 0; j < face.mNumIndices; j++)
				{
					m_indices.emplace_back(face.mIndices[j]);
				}
			}
		}

		// Bones
		if (aiMesh.HasBones())
		{
			m_boneWeights.resize(aiMesh.mNumVertices);

			for (unsigned i = 0; i < aiMesh.mNumBones; i++)
			{
				const auto aiBone = aiMesh.mBones[i];

				// look up bone in model's hierarchy or add it if not found
				unsigned boneIdx = m_model.GetBonesCount();
				const std::string boneName(aiBone->mName.C_Str());

				if (m_model.HasBone(boneName))
				{
					boneIdx = m_model.GetBoneIdx(boneName);
				}
				else
				{
					auto offsetMatrix = reinterpret_cast<math::Matrix4&>(aiBone->mOffsetMatrix);
					offsetMatrix = offsetMatrix.Transpose(); // assimp use row-major matrices

					m_model.m_bones.emplace_back(std::make_unique<Bone>(boneName, boneIdx, offsetMatrix));
					m_model.m_bonesMapping.emplace(boneName, boneIdx);
				}

				for (unsigned i = 0; i < aiBone->mNumWeights; i++)
				{
					const auto& aiVertexWeight = aiBone->mWeights[i];
					auto& boneWeight = m_boneWeights[aiVertexWeight.mVertexId];
					boneWeight.Push(Bone::VertexWeight(aiVertexWeight.mWeight, boneIdx));
				}
			}
		}
	}

	Mesh::~Mesh() = default;

	//-------------------------------------------------------------------------

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
		auto hr = m_model.GetApplication().GetDevice()->CreateBuffer(
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
