#pragma once
#include "library/NonCopyable.h"
#include "library/Model/Bone.h"
#include "library/Model/TextureType.h"
#include "library/Common.h"
#include "library/Render/PrimitiveData.h"
#include "library/DxForward.h"

#include <d3dcommon.h>

struct aiMesh;
struct ID3D11Buffer;

namespace library
{
struct TangentBinormal
{
	TangentBinormal(const math::Vector3& tangent, const math::Vector3& binormal)
		: tangent(tangent)
		, binormal(binormal)
	{}

	math::Vector3 tangent;
	math::Vector3 binormal;
};

using TangentBinormalArray = DynArray<TangentBinormal>;

//////////////////////////////////////////////////////////////////////////

class ModelMaterial;

class Mesh : public NonCopyable<Mesh>
{
	friend class Model;

public:
	~Mesh();

	Model& GetModel() { return m_model; }
	ModelMaterial& GetMaterial() const { return m_material; }

	const std::string& GetName() const { return m_name; }

	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const { return m_primitiveTopology; }

	// no positions = no vertices
	bool HasVertices() const { return HasPositions(); }
	std::size_t GetVerticesCount() const { return GetPositionsCount(); }

	//  positions
	bool HasPositions() const { return !m_positions.empty(); }
	std::size_t GetPositionsCount() const { return m_positions.size(); }
	const DynArray<math::Vector3>& GetPositions() const { return m_positions; }

	// normals
	bool HasNormals() const { return !m_normals.empty(); }
	std::size_t GetNormalsCount() const { return m_normals.size(); }
	const DynArray<math::Vector3>& GetNormals() const { return m_normals; }

	// tangents & binormals
	bool HasTangentBinormals() const { return !m_tangentBinormals.empty(); }
	std::size_t GetTangentBinormalsCount() const { return m_tangentBinormals.size(); }
	const TangentBinormalArray& GetTangentBinormals() const { return m_tangentBinormals; }

	// textures coordinates
	bool HasTexturesCoordinates() const { return !m_texturesCoordinates.empty(); }
	std::size_t GetTexturesCoordinatesCount() const { return m_texturesCoordinates.size(); }
	const DynArray<DynArray<math::Vector3>>& GetTexturesCoordinates() const { return m_texturesCoordinates; }
	const DynArray<math::Vector3>& GetTextureCoordinates(const unsigned textureIdx) const;

	// vertices colors
	bool HasVerticesColors() const { return !m_verticesColors.empty(); }
	std::size_t GetVerticesColorsCount() const { return m_verticesColors.size(); }
	const DynArray<DynArray<math::Color>>& GetVerticesColors() const { return m_verticesColors; }
	const DynArray<math::Color>& GetVertexColors(const unsigned vertexIdx) const;

	// default color will be used
	void Visualize(const bool randomColored = false);

	// indices
	bool HasIndices() const { return !m_indices.empty(); }
	std::size_t GetIndicesCount() const { return m_indices.size(); }
	const DynArray<unsigned>& GetIndices() const { return m_indices; }

	// faces
	bool HasFaces() const { return !!m_facesCount; }
	std::size_t GetFacesCount() const { return m_facesCount; }

	// bone weights
	bool HasBoneWeights() const { return !m_boneWeights.empty(); }
	const DynArray<Bone::VertexWeights>& GetBoneWeights() const { return m_boneWeights; }

	DynArray<ComPtr<ID3D11ShaderResourceView>> LoadTextures(const TextureType::Type textureType) const;

	IndexBufferData CreateIndexBufferData() const;
	template<typename VertexType>
	VertexBufferData CreateVertexBufferData() const;
	template<typename VertexType>
	PrimitiveData CreatePrimitiveData() const;

private:
	Mesh(Model& model, const aiMesh& aiMesh);

	ID3D11Device1* GetDevice() const;

	Model& m_model;
	ModelMaterial& m_material;

	std::string m_name;

	D3D_PRIMITIVE_TOPOLOGY m_primitiveTopology;

	DynArray<math::Vector3> m_positions;
	DynArray<math::Vector3> m_normals;
	TangentBinormalArray m_tangentBinormals;
	DynArray<DynArray<math::Vector3>> m_texturesCoordinates;
	DynArray<DynArray<math::Color>> m_verticesColors;

	std::size_t m_facesCount;
	DynArray<unsigned> m_indices;

	DynArray<Bone::VertexWeights> m_boneWeights;
};

//-------------------------------------------------------------------------

template<typename VertexType>
VertexBufferData Mesh::CreateVertexBufferData() const
{
	if (!HasVertices())
		return VertexBufferData();

	DynArray<VertexType> vertices;
	const auto verticesCount = GetVerticesCount();
	vertices.resize(verticesCount);

	static_assert(VertexType::ElementsType & VertexElementType::Position,
		"Can't create VertexBuffer for vertex type without position");

	for (unsigned i = 0; i < verticesCount; i++)
	{
		vertices[i].position = math::Vector4(m_positions[i], 1.f);

		if constexpr (bool(VertexType::ElementsType & VertexElementType::Texture))
			vertices[i].textureCoordinate = m_texturesCoordinates[0][i].xy;

		if constexpr (bool(VertexType::ElementsType & VertexElementType::Normal))
			vertices[i].normal = m_normals[i];

		if constexpr (bool(VertexType::ElementsType & VertexElementType::Tangent))
			vertices[i].tangent = m_tangentBinormals[i].tangent;

		if constexpr (bool(VertexType::ElementsType & VertexElementType::Color))
		{
			if (HasVerticesColors())
			{
				vertices[i].color = m_verticesColors[0][i];
			}
			else
			{
				vertices[i].color = colors::White;
			}
		}

		if constexpr (bool(VertexType::ElementsType & VertexElementType::Skinned))
		{
			const auto& vertexWeights = m_boneWeights[i];

			for (unsigned j = 0, count = vertexWeights.GetCount(); j < count; j++)
			{
				vertices[i].weights[j] = vertexWeights[j].weight;
				vertices[i].boneIndices[j] = vertexWeights[j].boneIndex;
			}
		}
	}

	return VertexBufferData(GetDevice(), MakeArrayBuffer(vertices));
}

//-------------------------------------------------------------------------

template<typename VertexType>
PrimitiveData Mesh::CreatePrimitiveData() const
{
	return PrimitiveData(m_primitiveTopology, CreateVertexBufferData<VertexType>(), CreateIndexBufferData());
}

} // namespace library
