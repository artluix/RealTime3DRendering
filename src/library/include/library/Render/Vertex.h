#pragma once
#include "library/Math/Color.h"
#include "library/Common.h"
#include "library/Render/VertexElement.h"

namespace library
{
template<int ... VertexElementTypes>
struct Vertex : VertexElement<VertexElementTypes>...
{
	constexpr Vertex() = default;
	constexpr Vertex(VertexElement<VertexElementTypes>&&... elements)
		: VertexElement<VertexElementTypes>(std::forward<VertexElement<VertexElementTypes>>(elements))... {}

	static constexpr auto ElementsType = (VertexElementTypes | ...);
	static constexpr auto ElementDescriptions = (VertexElement<VertexElementTypes>::Description + ...);
};

//-------------------------------------------------------------------------
// extern template declaration
//-------------------------------------------------------------------------

extern template struct Vertex<VertexElementType::Position>;
extern template struct Vertex<VertexElementType::Position, VertexElementType::Color>;
extern template struct Vertex<VertexElementType::Position, VertexElementType::Size>;
extern template struct Vertex<VertexElementType::Position, VertexElementType::Texture>;
extern template struct Vertex<VertexElementType::Position, VertexElementType::Texture, VertexElementType::Normal>;
extern template struct Vertex<VertexElementType::Position, VertexElementType::Texture, VertexElementType::Normal, VertexElementType::Tangent>;
extern template struct Vertex<VertexElementType::Position, VertexElementType::Texture, VertexElementType::Normal, VertexElementType::BoneIndices, VertexElementType::Weights>;

//-------------------------------------------------------------------------
// aliases
//-------------------------------------------------------------------------

using VertexPosition = Vertex<VertexElementType::Position>;
using VertexPositionColor = Vertex<VertexElementType::Position, VertexElementType::Color>;
using VertexPositionSize = Vertex<VertexElementType::Position, VertexElementType::Size>;
using VertexPositionTexture = Vertex<VertexElementType::Position, VertexElementType::Texture>;
using VertexPositionTextureNormal = Vertex<VertexElementType::Position, VertexElementType::Texture, VertexElementType::Normal>;
using VertexPositionTextureNormalTangent = Vertex<VertexElementType::Position, VertexElementType::Texture, VertexElementType::Normal, VertexElementType::Tangent>;
using VertexPositionTextureNormalSkinned = Vertex<VertexElementType::Position, VertexElementType::Texture, VertexElementType::Normal, VertexElementType::BoneIndices, VertexElementType::Weights>;

} // namespace library
