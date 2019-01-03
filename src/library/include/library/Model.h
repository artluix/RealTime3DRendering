#pragma once
#include "library/NonCopyable.hpp"

#include<string>
#include<vector>

namespace library
{
	namespace fs
	{
		class Path;
	} // namespace fs

	class Application;
	class Mesh;
	class ModelMaterial;

	class Model : public NonCopyable<Model>
	{
	public:
		explicit Model(const Application& app, const fs::Path& filePath, const bool flipUVs = false);
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
		const Application& m_app;

		std::vector<Mesh*> m_meshes;
		std::vector<ModelMaterial*> m_materials;
	};

} // namespace library

