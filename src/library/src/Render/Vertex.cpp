#include "StdAfx.h"
#include "library/Render/Vertex.h"

namespace library
{
//--------------------------------------------------------------------------
// template instantiations
//--------------------------------------------------------------------------

template struct VertexElement<VertexElementType::Position>;
template struct VertexElement<VertexElementType::Color>;
template struct VertexElement<VertexElementType::Size>;
template struct VertexElement<VertexElementType::Texture>;
template struct VertexElement<VertexElementType::Normal>;
template struct VertexElement<VertexElementType::Tangent>;
template struct VertexElement<VertexElementType::BoneIndices>;
template struct VertexElement<VertexElementType::Weights>;

//-------------------------------------------------------------------------

template struct Vertex<VertexElementType::Position>;
template struct Vertex<VertexElementType::Position, VertexElementType::Color>;
template struct Vertex<VertexElementType::Position, VertexElementType::Size>;
template struct Vertex<VertexElementType::Position, VertexElementType::Texture>;
template struct Vertex<VertexElementType::Position, VertexElementType::Texture, VertexElementType::Normal>;
template struct Vertex<VertexElementType::Position, VertexElementType::Texture, VertexElementType::Normal, VertexElementType::Tangent>;
template struct Vertex<VertexElementType::Position, VertexElementType::Texture, VertexElementType::Normal, VertexElementType::BoneIndices, VertexElementType::Weights>;

} // namespace library
