#include "StdAfx.h"
#include "library/Model/Model.h"

#include "library/Model/Mesh.h"
#include "library/Model/ModelMaterial.h"
#include "library/Model/Bone.h"
#include "library/Model/AnimationClip.h"

#include "library/Application.h"
#include "library/Path.h"
#include "library/Logger.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace library
{
Model::Model(const Application& app, const std::string& name, const bool flipUVs /*= false*/) : m_app(app)
{
	Assimp::Importer importer;

	unsigned flags = aiProcess_Triangulate |
					 aiProcess_JoinIdenticalVertices |
					 aiProcess_SortByPType |
					 aiProcess_FlipWindingOrder;

	if (flipUVs)
		flags |= aiProcess_FlipUVs;

	auto filename = name;
	if (!Path(filename).GetExt())
		filename += ".obj";

	const auto path = app.GetModelsPath() + Path(filename);

	if (const auto aiScene = importer.ReadFile(path.GetString(), flags))
	{
		if (aiScene->HasMaterials())
		{
			m_materials.reserve(aiScene->mNumMaterials);

			for (unsigned i = 0; i < aiScene->mNumMaterials; i++)
			{
				m_materials.emplace_back(new ModelMaterial(*this, *aiScene->mMaterials[i]));
			}
		}

		if (aiScene->HasMeshes())
		{
			m_meshes.reserve(aiScene->mNumMeshes);

			for (unsigned i = 0; i < aiScene->mNumMeshes; i++)
			{
				m_meshes.emplace_back(new Mesh(*this, *aiScene->mMeshes[i]));
			}
		}

		if (aiScene->HasAnimations())
		{
			assert(!!aiScene->mRootNode);
			m_rootNode = BuildSkeleton(*aiScene->mRootNode);

			m_animations.reserve(aiScene->mNumAnimations);

			for (unsigned i = 0; i < aiScene->mNumAnimations; i++)
			{
				const auto& animation =
					m_animations.emplace_back(new AnimationClip(*this, *aiScene->mAnimations[i]));
				m_animationsMapping.emplace(animation->GetName(), i);
			}
		}
	}
	else
	{
		Logger::Error(importer.GetErrorString());
		assert(false);
	}

#if defined DEBUG || defined _DEBUG
	ValidateModel();
#endif
}

Model::~Model() = default;

//-------------------------------------------------------------------------
// meshes
//-------------------------------------------------------------------------

const Mesh& Model::GetMesh(const unsigned meshIdx) const
{
	return *m_meshes.at(meshIdx);
}

Mesh& Model::GetMesh(const unsigned meshIdx)
{
	return *m_meshes.at(meshIdx);
}

//-------------------------------------------------------------------------
// materials
//-------------------------------------------------------------------------

const ModelMaterial& Model::GetMaterial(const unsigned materialIdx) const
{
	return *m_materials.at(materialIdx);
}

ModelMaterial& Model::GetMaterial(const unsigned materialIdx)
{
	return *m_materials.at(materialIdx);
}

//-------------------------------------------------------------------------
// bones
//-------------------------------------------------------------------------

bool Model::HasBone(const std::string& boneName) const
{
	return m_bonesMapping.find(boneName) != m_bonesMapping.end();
}

unsigned Model::GetBoneIdx(const std::string& boneName) const
{
	assert(HasBone(boneName));
	return m_bonesMapping.at(boneName);
}

const Bone& Model::GetBone(const std::string& boneName) const
{
	return GetBone(GetBoneIdx(boneName));
}

// Bone& Model::GetBone(const std::string& boneName)
//{
//	return GetBone(GetBoneIdx(boneName));
//}

//-------------------------------------------------------------------------

const Bone& Model::GetBone(const unsigned boneIdx) const
{
	assert(boneIdx < m_bones.size());
	return *m_bones.at(boneIdx);
}

// Bone& Model::GetBone(const unsigned boneIdx)
//{
//	assert(boneIdx < m_bones.size());
//	return *m_bones.at(boneIdx);
//}

//-------------------------------------------------------------------------
// animations
//-------------------------------------------------------------------------

bool Model::HasAnimation(const std::string& animationName) const
{
	return m_animationsMapping.find(animationName) != m_animationsMapping.end();
}

unsigned Model::GetAnimationIdx(const std::string& animationName) const
{
	assert(HasAnimation(animationName));
	return m_animationsMapping.at(animationName);
}

const AnimationClip& Model::GetAnimation(const std::string& animationName) const
{
	return GetAnimation(GetAnimationIdx(animationName));
}

// AnimationClip& Model::GetAnimation(const std::string& animationName)
//{
//	return GetAnimation(GetAnimationIdx(animationName));
//}

//-------------------------------------------------------------------------

const AnimationClip& Model::GetAnimation(const unsigned animationIdx) const
{
	assert(animationIdx < m_animations.size());
	return *m_animations.at(animationIdx);
}

// AnimationClip& Model::GetAnimation(const unsigned animationIdx)
//{
//	assert(animationIdx < m_animations.size());
//	return *m_animations.at(animationIdx);
//}

//-------------------------------------------------------------------------

SceneNodePtr Model::BuildSkeleton(const aiNode& aiNode, const SceneNode* parentNode /* = nullptr */)
{
	// static unsigned indent = 0;

	SceneNodePtr node;

	const std::string nodeName = aiNode.mName.C_Str();

	// uncomment to output hierarchy
	// Logger::Log(Logger::Level::Debug, "%sNode: %s", std::string(indent * 4, ' ').c_str(), nodeName.c_str());

	if (HasBone(nodeName))
	{
		node = std::static_pointer_cast<SceneNode>(m_bones[GetBoneIdx(nodeName)]);
	}
	else
	{
		node = std::make_shared<SceneNode>(nodeName);
	}

	const auto transform = reinterpret_cast<const math::Matrix4&>(aiNode.mTransformation);
	node->SetTransform(transform.Transpose()); // assimp use row-major matrices
	node->SetParent(parentNode);

	// indent++;

	for (unsigned i = 0; i < aiNode.mNumChildren; i++)
	{
		node->AddChild(BuildSkeleton(*aiNode.mChildren[i], node.get()));
	}

	// indent--;

	return node;
}

void Model::ValidateModel() const
{
	// Validate bone weights
	for (const auto& mesh : m_meshes)
	{
		for (const auto& boneWeight : mesh->GetBoneWeights())
		{
			float totalWeight{0.f};

			for (unsigned i = 0; i < boneWeight.GetCount(); i++)
			{
				const auto& vw = boneWeight[i];
				totalWeight += vw.weight;
				assert(vw.boneIndex >= 0 && vw.boneIndex < m_bones.size());
			}

			assert(totalWeight >= 0.95f && totalWeight <= 1.05f);
		}
	}
}
} // namespace library
