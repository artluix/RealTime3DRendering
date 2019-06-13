#pragma once
#include "library/NonCopyable.hpp"
#include "library/Model/Bone.h"
#include "library/Model/TextureType.h"
#include "library/Common.h"
#include "library/BufferData.h"
#include "library/DirectXForwardDeclarations.h"

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

using TangentBinormalVector = std::vector<TangentBinormal>;

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

	//  vertices
	bool HasVertices() const { return !m_vertices.empty(); }
	std::size_t GetVerticesCount() const { return m_vertices.size(); }
	const std::vector<math::Vector3>& GetVertices() const { return m_vertices; }

	// normals
	bool HasNormals() const { return !m_normals.empty(); }
	std::size_t GetNormalsCount() const { return m_normals.size(); }
	const std::vector<math::Vector3>& GetNormals() const { return m_normals; }

	// tangents & binormals
	bool HasTangentBinormals() const { return !m_tangentBinormals.empty(); }
	std::size_t GetTangentBinormalsCount() const { return m_tangentBinormals.size(); }
	const TangentBinormalVector& GetTangentBinormals() const { return m_tangentBinormals; }

	// textures coordinates
	bool HasTexturesCoordinates() const { return !m_texturesCoordinates.empty(); }
	std::size_t GetTexturesCoordinatesCount() const { return m_texturesCoordinates.size(); }
	const std::vector<std::vector<math::Vector3>>& GetTexturesCoordinates() const { return m_texturesCoordinates; }
	const std::vector<math::Vector3>& GetTextureCoordinates(const unsigned textureIdx) const;

	// vertices colors
	bool HasVerticesColors() const { return !m_verticesColors.empty(); }
	std::size_t GetVerticesColorsCount() const { return m_verticesColors.size(); }
	const std::vector<std::vector<math::Color>>& GetVerticesColors() const { return m_verticesColors; }
	const std::vector<math::Color>& GetVertexColors(const unsigned vertexIdx) const;

	// indices
	bool HasIndices() const { return !m_indices.empty(); }
	std::size_t GetIndicesCount() const { return m_indices.size(); }
	const std::vector<unsigned>& GetIndices() const { return m_indices; }

	// faces
	bool HasFaces() const { return !!m_facesCount; }
	std::size_t GetFacesCount() const { return m_facesCount; }

	// bone weights
	bool HasBoneWeights() const { return !m_boneWeights.empty(); }
	const std::vector<Bone::VertexWeights>& GetBoneWeights() const { return m_boneWeights; }

	std::vector<ComPtr<ID3D11ShaderResourceView>> LoadTextures(const TextureType::Type textureType) const;

	IndexBufferData CreateIndexBufferData() const;

private:
	Mesh(Model& model, const aiMesh& aiMesh);

	Model& m_model;
	ModelMaterial& m_material;

	std::string m_name;

	D3D_PRIMITIVE_TOPOLOGY m_primitiveTopology;

	std::vector<math::Vector3> m_vertices;
	std::vector<math::Vector3> m_normals;
	TangentBinormalVector m_tangentBinormals;
	std::vector<std::vector<math::Vector3>> m_texturesCoordinates;
	std::vector<std::vector<math::Color>> m_verticesColors;

	std::size_t m_facesCount;
	std::vector<unsigned> m_indices;

	std::vector<Bone::VertexWeights> m_boneWeights;
};
} // namespace library
