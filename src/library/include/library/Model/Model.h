#pragma once
#include "library/Common.h"

#include <string>
#include <map>
#include <memory>

struct aiNode;

namespace library
{
class Application;
class ModelMaterial;
class Bone;
class SceneNode;
class AnimationClip;

//-------------------------------------------------------------------------

class Model : public NonCopyable<Model>
{
	friend class Mesh;

public:
	Model(const Application& app, const std::string& name, const bool flipUVs = false);
	~Model();

	const Application& GetApp() const { return m_app; }

	// meshes
	bool HasMeshes() const { return !m_meshes.empty(); }
	unsigned GetMeshesCount() const { return static_cast<unsigned>(m_meshes.size()); }
	const Mesh& GetMesh(const unsigned meshIdx) const;
	Mesh& GetMesh(const unsigned meshIdx);

	// materials
	bool HasMaterials() const { return !m_materials.empty(); }
	unsigned GetMaterialsCount() const { return static_cast<unsigned>(m_materials.size()); }
	const ModelMaterial& GetMaterial(const unsigned materialIdx) const;
	ModelMaterial& GetMaterial(const unsigned materialIdx);

	// bones
	bool HasBones() const { return !m_bones.empty(); }
	unsigned GetBonesCount() const { return static_cast<unsigned>(m_bones.size()); }
	bool HasBone(const std::string& boneName) const;

	unsigned GetBoneIdx(const std::string& boneName) const;
	const Bone& GetBone(const std::string& boneName) const;
	// Bone& GetBone(const std::string& boneName);

	const Bone& GetBone(const unsigned boneIdx) const;
	// Bone& GetBone(const unsigned boneIdx);

	// animations
	bool HasAnimations() const { return !m_animations.empty(); }
	unsigned GetAnimationsCount() const { return static_cast<unsigned>(m_animations.size()); }
	bool HasAnimation(const std::string& animationName) const;

	unsigned GetAnimationIdx(const std::string& animationName) const;
	const AnimationClip& GetAnimation(const std::string& animationName) const;
	// AnimationClip& GetAnimation(const std::string& animationName);

	const AnimationClip& GetAnimation(const unsigned animationIdx) const;
	// AnimationClip& GetAnimation(const unsigned animationIdx);

	const SceneNode* GetRootNode() const { return m_rootNode.get(); }

private:
	using MaterialPtr = std::unique_ptr<ModelMaterial>;
	using MeshPtr = std::unique_ptr<Mesh>;
	using SceneNodePtr = std::shared_ptr<SceneNode>;
	using BonePtr = std::shared_ptr<Bone>;
	using AnimationClipPtr = std::shared_ptr<AnimationClip>;

	SceneNodePtr BuildSkeleton(const aiNode& aiNode, const SceneNode* parentNode = nullptr);
	void ValidateModel() const;

	const Application& m_app;

	DynArray<MeshPtr> m_meshes;
	DynArray<MaterialPtr> m_materials;

	DynArray<BonePtr> m_bones;
	std::map<std::string, unsigned> m_bonesMapping;

	DynArray<AnimationClipPtr> m_animations;
	std::map<std::string, unsigned> m_animationsMapping;

	SceneNodePtr m_rootNode;
};
} // namespace library
