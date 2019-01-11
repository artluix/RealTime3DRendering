#pragma once
#include "library/NonCopyable.hpp"
#include "library/CommonTypes.h"

#include <DirectXMath.h>
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
		{
		}

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
		const std::vector<DirectX::XMFLOAT3Vector>& GetTexturesCoordinates() const { return m_texturesCoordinates; }
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
		bool HasFaces() const { return m_facesCount > 0; }
		unsigned GetFacesCount() const { return m_facesCount; }

		ComPtr<ID3D11Buffer> CreateIndexBuffer() const;

	private:
		explicit Mesh(Model& model, aiMesh& mesh);

		Model& m_model;
		ModelMaterial& m_material;

		std::string m_name;

		DirectX::XMFLOAT3Vector m_vertices;
		DirectX::XMFLOAT3Vector m_normals;
		TangentBinormalVector m_tangentBinormals;
		std::vector<DirectX::XMFLOAT3Vector> m_texturesCoordinates;
		std::vector<DirectX::XMFLOAT4Vector> m_verticesColors;

		unsigned m_facesCount;

		std::vector<unsigned> m_indices;
	};
} // namespace library
