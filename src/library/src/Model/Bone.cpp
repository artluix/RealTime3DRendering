#include "StdAfx.h"
#include "library/Model/Bone.h"

namespace library
{
Bone::VertexWeights::VertexWeights()
{
	m_weights.reserve(MaxPerVertex);
}

void Bone::VertexWeights::Push(const VertexWeight& vw)
{
	if (GetCount() == MaxPerVertex)
	{
		assert("Maximum number of bone weights per vertex exceeded." && false);
	}

	m_weights.push_back(vw);
}

const Bone::VertexWeight& Bone::VertexWeights::operator[](const unsigned idx) const
{
	assert(idx < m_weights.size());
	return m_weights[idx];
}

//-------------------------------------------------------------------------

Bone::Bone(const std::string& name, const unsigned index, const math::Matrix4& offsetTransform)
	: SceneNode(name)
	, m_index(index)
	, m_offsetTransform(offsetTransform)
{}

void Bone::SetIndex(const unsigned index)
{
	m_index = index;
}
} // namespace library
