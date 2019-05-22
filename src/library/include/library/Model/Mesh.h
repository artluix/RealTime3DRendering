#pragma once
#include "library/NonCopyable.hpp"
#include "library/Model/Bone.h"
#include "library/Model/TextureType.h"
#include "library/CommonTypes.h"
#include "library/BufferData.h"
#include "library/DirectXForwardDeclarations.h"

#include <DirectXMath.h>
#include <d3dcommon.h>
#include <string>
#include <vector>

struct aiMesh;
struct ID3D11Buffer;

namespace DirectX
{
using XMFLOAT3Vector = std::vector<DirectX::XMFLOAT3>;
using XMFLOAT4Vector = std::vector<DirectX::XMFLOAT4>;
} // namespace DirectX

namespace library
{
struct TangentBinormal
{
	TangentBinormal(const DirectX::XMFLOAT3& tangent, const DirectX::XMFLOAT3& binormal)
		: tangent(tangent)
		, binormal(binormal)
	{}

	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 binormal;
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
	const DirectX::XMFLOAT3Vector& GetVertices() const { return m_vertices; }

	// normals
	bool HasNormals() const { return !m_normals.empty(); }
	std::size_t GetNormalsCount() const { return m_normals.size(); }
	const DirectX::XMFLOAT3Vector& GetNormals() const { return m_normals; }

	// tangents & binormals
	bool HasTangentBinormals() const { return !m_tangentBinormals.empty(); }
	std::size_t GetTangentBinormalsCount() const { return m_tangentBinormals.size(); }
	const TangentBinormalVector& GetTangentBinormals() const { return m_tangentBinormals; }

	// textures coordinates
	bool HasTexturesCoordinates() const { return !m_texturesCoordinates.empty(); }
	std::size_t GetTexturesCoordinatesCount() const { return m_texturesCoordinates.size(); }
	const std::vector<DirectX::XMFLOAT3Vector>& GetTexturesCoordinates() const
	{
		return m_texturesCoordinates;
	}
	const DirectX::XMFLOAT3Vector& GetTextureCoordinates(const unsigned textureIdx) const;

	// vertices colors
	bool HasVerticesColors() const { return !m_verticesColors.empty(); }
	std::size_t GetVerticesColorsCount() const { return m_verticesColors.size(); }
	const std::vector<DirectX::XMFLOAT4Vector>& GetVerticesColors() const { return m_verticesColors; }
	const DirectX::XMFLOAT4Vector& GetVertexColors(const unsigned vertexIdx) const;

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

	DirectX::XMFLOAT3Vector m_vertices;
	DirectX::XMFLOAT3Vector m_normals;
	TangentBinormalVector m_tangentBinormals;
	std::vector<DirectX::XMFLOAT3Vector> m_texturesCoordinates;
	std::vector<DirectX::XMFLOAT4Vector> m_verticesColors;

	std::size_t m_facesCount;
	std::vector<unsigned> m_indices;

	std::vector<Bone::VertexWeights> m_boneWeights;
};
} // namespace library
