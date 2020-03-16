#pragma once
#include "library/Model/SceneNode.h"
//#include "library/Render/VertexTypes.h"

namespace library
{
class Bone : public SceneNode
{
	RTTI_CLASS(Bone, SceneNode)

public:
	struct VertexWeight
	{
		float weight;
		unsigned boneIndex;

		constexpr VertexWeight(const float weight, const unsigned boneIndex)
			: weight(weight), boneIndex(boneIndex)
		{}
	};

	//-------------------------------------------------------------------------

	class VertexWeights
	{
	public:
		VertexWeights();

		void Push(const VertexWeight& vw);
		void Push(VertexWeight&& vw) { Push(vw); }

		unsigned GetCount() const { return static_cast<unsigned>(m_weights.size()); }

		const VertexWeight& operator[](const unsigned idx) const;

		static inline constexpr std::size_t MaxPerVertex = 4;

	private:
		std::vector<VertexWeight> m_weights;
	};

	//-------------------------------------------------------------------------

	Bone(const std::string& name, const unsigned index, const math::Matrix4& offsetTransform);

	unsigned GetIndex() const { return m_index; }
	void SetIndex(const unsigned index);

	const math::Matrix4& GetOffsetTransform() const { return m_offsetTransform; }

private:
	unsigned m_index; // index int model's bone
	math::Matrix4 m_offsetTransform;
};
} // namespace library
