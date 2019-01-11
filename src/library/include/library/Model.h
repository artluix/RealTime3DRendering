#pragma once
#include "library/NonCopyable.hpp"

#include <string>
#include <vector>
#include <memory>

namespace library
{
	class Application;
	class Path;
	class Mesh;
	class ModelMaterial;

	class Model : public NonCopyable<Model>
	{
	public:
		explicit Model(const Application& app, const Path& filePath, const bool flipUVs = false);
		~Model();

		const Application& GetApplication() const { return m_app; }

		// meshes
		bool HasMeshes() const { return !m_meshes.empty(); }
		std::size_t GetMeshesCount() const { return m_meshes.size(); }
		Mesh& GetMesh(const unsigned meshIdx) const;

		// materials
		bool HasMaterials() const { return !m_materials.empty(); }
		std::size_t GetMaterialsCount() const { return m_materials.size(); }
		ModelMaterial& GetMaterial(const unsigned materialIdx) const;

	private:
		using MaterialPtr = std::unique_ptr<ModelMaterial>;
		using MeshPtr = std::unique_ptr<Mesh>;

		const Application& m_app;

		std::vector<MeshPtr> m_meshes;
		std::vector<MaterialPtr> m_materials;
	};
} // namespace library

