#pragma once
#include "library/NonCopyable.h"
#include "library/RTTI.h"
#include "library/Math/Matrix.h"

#include <string>
#include <memory>

namespace library
{
class SceneNode;
using SceneNodePtr = std::shared_ptr<SceneNode>;

class SceneNode : public NonCopyable<SceneNode>
{
	RTTI_CLASS_BASE(SceneNode)

public:
	explicit SceneNode(const std::string& name, const math::Matrix4& transform = math::Matrix4::Identity);

	virtual ~SceneNode();

	const std::string& GetName() const { return m_name; }

	const math::Matrix4& GetTransform() const { return m_transform; }
	void SetTransform(const math::Matrix4& transform);

	bool HasParent() const { return !!m_parent; }
	const SceneNode& GetParentRef() const { return *m_parent; }
	const SceneNode* GetParent() const { return m_parent; }
	void SetParent(const SceneNode* parent);

	unsigned GetChildrenCount() const { return static_cast<unsigned>(m_children.size()); }
	bool HasChild(const std::string& childName) const;
	const SceneNode& GetChild(const std::string& childName) const;
	const SceneNode& GetChild(const unsigned childIdx) const;
	void AddChild(SceneNodePtr&& child);

protected:
	std::string m_name;
	math::Matrix4 m_transform;

	const SceneNode* m_parent = nullptr;
	DynArray<SceneNodePtr> m_children;
};
} // namespace library
