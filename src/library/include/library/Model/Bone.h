#pragma once
#include "library/Model/SceneNode.h"
#include "library/VertexTypes.h"

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

			constexpr explicit VertexWeight(const float weight, const unsigned boneIndex)
				: weight(weight)
				, boneIndex(boneIndex)
			{
			}
		};

		//-------------------------------------------------------------------------

		class VertexWeights
		{
		public:
			using const_iterator = std::vector<VertexWeight>::const_iterator;

			explicit VertexWeights();

			void Push(const VertexWeight& vw);
			void Push(VertexWeight&& vw) { Push(vw); }

			const_iterator begin() const noexcept { return m_weights.cbegin(); }
			const_iterator end() const noexcept { return m_weights.cend(); }

			std::size_t Size() const { return m_weights.size(); }

			static inline constexpr std::size_t MaxPerVertex = 4;

		private:
			std::vector<VertexWeight> m_weights;
		};

		//-------------------------------------------------------------------------

		explicit Bone(const std::string& name, const unsigned index, const math::Matrix4& offsetTransform);

		unsigned GetIndex() const { return m_index; }
		void SetIndex(const unsigned index);

		const math::Matrix4& GetOffsetTransform() const { return m_offsetTransform; }

	private:
		unsigned m_index; // index int model's bone
		math::Matrix4 m_offsetTransform;
	};
} // namespace library
