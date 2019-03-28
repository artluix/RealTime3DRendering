#include "StdAfx.h"
#include "library/Model/SceneNode.h"

#include <cassert>

namespace library
{
	namespace
	{
		class NodeFinder
		{
		public:
			explicit NodeFinder(const std::string& nodeName)
				: m_nodeName(nodeName)
			{
			}

			bool operator () (const SceneNodePtr& node) const
			{
				return node->GetName() == m_nodeName;
			}

		private:
			const std::string& m_nodeName;
		};
	}

	SceneNode::SceneNode(
		const std::string& name,
		const math::Matrix4& transform /*= math::Matrix4::Identity */
	)
		: m_name(name)
		, m_transform(transform)
	{
	}

	SceneNode::~SceneNode() = default;

	//-------------------------------------------------------------------------

	void SceneNode::SetParent(const SceneNode* parent)
	{
		m_parent = parent;
	}

	void SceneNode::SetTransform(const math::Matrix4& transform)
	{
		m_transform = transform;
	}

	//-------------------------------------------------------------------------

	bool SceneNode::HasChild(const std::string& childName) const
	{
		auto it = std::find_if(m_children.begin(), m_children.end(), NodeFinder(childName));
		return it != m_children.end();
	}

	const SceneNode& SceneNode::GetChild(const std::string& childName) const
	{
		auto it = std::find_if(m_children.begin(), m_children.end(), NodeFinder(childName));
		assert(it != m_children.end());
		return **it;
	}

	const SceneNode& SceneNode::GetChild(const unsigned childIdx) const
	{
		assert(childIdx < m_children.size());
		return *m_children[childIdx];
	}

	void SceneNode::AddChild(SceneNodePtr&& child)
	{
		assert(!HasChild(child->GetName()));
		m_children.emplace_back(std::move(child));
	}
} // namespace library
