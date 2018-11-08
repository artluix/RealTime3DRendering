#pragma once
#include "library/NonCopyable.hpp"

#include<memory>
#include<string>
#include<vector>

namespace library
{
	class Application;

	class Mesh;
	using MeshPtr = std::shared_ptr<Mesh>;

	class Material;
	using MaterialPtr = std::shared_ptr<Material>;

	class Model : public NonCopyable<Model>
	{
	public:
		explicit Model(const Application& app, const std::string& filename, const bool flipUVs = false);
		~Model() = default;

		const Application& GetApplication() const { return m_app; }

		bool HasMeshes() const { return !m_meshes.empty(); }
		bool HasMaterials() const { return !m_materials.empty(); }

		const std::vector<MeshPtr>& GetMeshes() const { return m_meshes; }
		const std::vector<MaterialPtr>& GetMaterials() const { return m_materials; }

		MeshPtr GetMesh(const unsigned meshIdx) const;
		MaterialPtr GetMaterial(const unsigned materialIdx) const;

	private:
		const Application& m_app;

		std::vector<MeshPtr> m_meshes;
		std::vector<MaterialPtr> m_materials;
	};

	using ModelPtr = std::shared_ptr<Model>;

} // namespace library

