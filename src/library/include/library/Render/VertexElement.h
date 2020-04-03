#pragma once
#include "library/Math/Color.h"
#include "library/Math/Matrix.h"
#include "library/Render/Misc.h"

namespace library
{
namespace VertexElementType
{
	enum Type : int
	{
		Position = 1,
		Color = 1 << 1,
		Size = 1 << 2,
		Texture = 1 << 3,
		Normal = 1 << 4,
		Tangent = 1 << 5,

		BoneIndices = 1 << 6,
		Weights = 1 << 7,
		Skinned = BoneIndices | Weights,
	};
} // namespace VertexElementType

//-------------------------------------------------------------------------
// VertexElement
//-------------------------------------------------------------------------

template<int Type>
struct VertexElement;

//-------------------------------------------------------------------------

#define DECLARE_VERTEX_ELEMENT_EX(VertexElementType, MemberType, memberName, SemanticName, Slot)																						\
template<>																																												\
struct VertexElement<VertexElementType>																																					\
{																																														\
	static_assert((VertexElementType & (VertexElementType - 1)) == 0, "Can't declare vertex element of combined type");																	\
																																														\
	MemberType memberName{};																																								\
																																														\
	constexpr VertexElement() = default;																																				\
	constexpr VertexElement(const MemberType& memberName) : memberName(memberName) {}																									\
																																														\
	static constexpr auto Type = VertexElementType;																																		\
	static constexpr InputElementDescArray<1> Description {{ #SemanticName, 0, dx::math::DXGI_FORMAT<MemberType>, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_##Slot##_DATA, 0 }};	\
};																																														\
extern template struct VertexElement<VertexElementType>;

#define DECLARE_VERTEX_ELEMENT(VertexElementType, MemberType, memberName, SemanticName) DECLARE_VERTEX_ELEMENT_EX(VertexElementType, MemberType, memberName, SemanticName, VERTEX)
#define DECLARE_VERTEXINSTANCE_ELEMENT(VertexElementType, MemberType, memberName, SemanticName) DECLARE_VERTEX_ELEMENT_EX(VertexElementType, MemberType, memberName, SemanticName, INSTANCE)

//-------------------------------------------------------------------------

DECLARE_VERTEX_ELEMENT(VertexElementType::Position, math::Vector4, position, POSITION)
DECLARE_VERTEX_ELEMENT(VertexElementType::Color, math::Color, color, COLOR)
DECLARE_VERTEX_ELEMENT(VertexElementType::Size, math::Vector2, size, SIZE)
DECLARE_VERTEX_ELEMENT(VertexElementType::Texture, math::Vector2, textureCoordinate, TEXCOORD)
DECLARE_VERTEX_ELEMENT(VertexElementType::Normal, math::Vector3, normal, NORMAL)
DECLARE_VERTEX_ELEMENT(VertexElementType::Tangent, math::Vector3, tangent, TANGENT)
DECLARE_VERTEX_ELEMENT(VertexElementType::BoneIndices, math::Vector4u, boneIndices, BONEINDICES)
DECLARE_VERTEX_ELEMENT(VertexElementType::Weights, math::Vector4, weights, WEIGHTS)

} // namespace library
