#include "StdAfx.h"
#include "library/Model.h"

#include "library/Application.h"
#include "library/Application.h"
#include "library/Exception.h"
#include "library/Mesh.h"
#include "library/Material.h"
#include "library/Path.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace library
{
	Model::Model(const Application& app, const filesystem::Path& filePath, const bool flipUVs /*= false*/)
		: m_app(app)
	{
		Assimp::Importer importer;

		unsigned flags =
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipWindingOrder;

		if (flipUVs)
			flags |= aiProcess_FlipUVs;

		if (const auto scene = importer.ReadFile(filePath.GetString(), flags))
		{
			if (scene->HasMaterials())
			{
				m_materials.reserve(scene->mNumMaterials);

				for (unsigned i = 0; i < scene->mNumMaterials; i++)
				{
					m_materials.emplace_back(new Material(*this, *(scene->mMaterials[i])));
				}
			}

			if (scene->HasMeshes())
			{
				m_meshes.reserve(scene->mNumMeshes);

				for (unsigned i = 0; i < scene->mNumMeshes; i++)
				{
					m_meshes.emplace_back(new Mesh(*this, *(scene->mMeshes[i])));
				}
			}
		}
		else
		{
			throw Exception(importer.GetErrorString());
		}
	}

	const MeshPtr& Model::GetMesh(const unsigned meshIdx) const
	{
		assert(meshIdx <= m_meshes.size());
		return m_meshes[meshIdx];
	}

	const MaterialPtr& Model::GetMaterial(const unsigned materialIdx) const
	{
		assert(materialIdx <= m_materials.size());
		return m_materials[materialIdx];
	}
} // namespace library
