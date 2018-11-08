#pragma once
#include "library/NonCopyable.hpp"
#include "library/Common.h"

#include <string>
#include <vector>
#include <DirectXMath.h>

struct aiMesh;
struct ID3D11Buffer;

namespace DirectX
{
	using XMFLOAT3Vector = std::vector<DirectX::XMFLOAT3>;
	using XMFLOAT4Vector = std::vector<DirectX::XMFLOAT4>;
} // namespace DirectX

namespace library
{
	class Material;
	using MaterialPtr = std::shared_ptr<Material>;

	class Mesh : public NonCopyable<Mesh>
	{
		friend class Model;

	public:
		~Mesh();

		Model& GetModel() { return m_model; }
		const MaterialPtr& GetMaterial() const { return m_material; }

		const std::string& GetName() const { return m_name; }

		const DirectX::XMFLOAT3Vector& GetVertices() const { return m_vertices; }
		const DirectX::XMFLOAT3Vector& GetNormals() const { return m_normals; }
		const DirectX::XMFLOAT3Vector& GetTangents() const { return m_tangents; }
		const DirectX::XMFLOAT3Vector& GetBinormals() const { return m_binormals; }
		const std::vector<DirectX::XMFLOAT3Vector>& GetTexturesCoordinates() const { return m_texturesCoordinates; }
		const std::vector<DirectX::XMFLOAT4Vector>& GetVerticesColors() const { return m_verticesColors; }

		const DirectX::XMFLOAT3Vector& GetTextureCoordinates(const unsigned textureIdx) const;
		const DirectX::XMFLOAT4Vector& GetVertexColors(const unsigned vertexIdx) const;

		unsigned GetFacesCount() const { return m_facesCount; }
		const std::vector<unsigned>& GetIndices() const { return m_indices; }

		ComPtr<ID3D11Buffer> CreateIndexBuffer();

	private:
		explicit Mesh(Model& model, aiMesh& mesh);

		Model& m_model;
		MaterialPtr m_material;

		std::string m_name;

		DirectX::XMFLOAT3Vector m_vertices;
		DirectX::XMFLOAT3Vector m_normals;
		DirectX::XMFLOAT3Vector m_tangents;
		DirectX::XMFLOAT3Vector m_binormals;
		std::vector<DirectX::XMFLOAT3Vector> m_texturesCoordinates;
		std::vector<DirectX::XMFLOAT4Vector> m_verticesColors;

		unsigned m_facesCount;

		std::vector<unsigned> m_indices;
	};
} // namespace library
