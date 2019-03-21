#include "StdAfx.h"
#include "library/Model/Bone.h"

#include "library/Exception.h"

namespace library
{
	Bone::VertexWeights::VertexWeights()
	{
		m_weights.reserve(MaxPerVertex);
	}

	void Bone::VertexWeights::Push(const VertexWeight& vw)
	{
		if (Size() == MaxPerVertex)
		{
			throw Exception("Maximum number of bone weights per vertex exceeded.");
		}

		m_weights.push_back(vw);
	}

	//-------------------------------------------------------------------------

	Bone::Bone(const std::string& name, const unsigned index, const math::Matrix4& offsetTransform)
		: SceneNode(name)
		, m_index(index)
		, m_offsetTransform(offsetTransform)
	{
	}

	void Bone::SetIndex(const unsigned index)
	{
		m_index = index;
	}
} // namespace library
