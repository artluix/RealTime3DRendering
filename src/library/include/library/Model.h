#pragma once
#include "library/NonCopyable.hpp"

#include<memory>
#include<string>
#include<vector>

namespace library
{
	namespace filesystem
	{
		class Path;
	} // namespace filesystem

	class Application;

	class Mesh;
	using MeshPtr = std::shared_ptr<Mesh>;

	class ModelMaterial;
	using ModelMaterialPtr = std::shared_ptr<ModelMaterial>;

	class Model : public NonCopyable<Model>
	{
	public:
		explicit Model(const Application& app, const filesystem::Path& filePath, const bool flipUVs = false);
		~Model() = default;

		const Application& GetApplication() const { return m_app; }

		// meshes
		bool HasMeshes() const { return !m_meshes.empty(); }
		std::size_t GetMeshesCount() const { return m_meshes.size(); }
		//const std::vector<MeshPtr>& GetMeshes() const { return m_meshes; }
		Mesh* GetMesh(const unsigned meshIdx) const;

		// materials
		bool HasMaterials() const { return !m_materials.empty(); }
		std::size_t GetMaterialsCount() const { return m_materials.size(); }
		//const std::vector<MaterialPtr>& GetMaterials() const { return m_materials; }
		ModelMaterial* GetMaterial(const unsigned materialIdx) const;

	private:
		const Application& m_app;

		std::vector<MeshPtr> m_meshes;
		std::vector<ModelMaterialPtr> m_materials;
	};

	using ModelPtr = std::shared_ptr<Model>;

} // namespace library

