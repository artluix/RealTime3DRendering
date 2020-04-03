#pragma once
#include <library/Render/Vertex.h>

// extend library vertex types
namespace library
{
namespace VertexElementType
{
	enum ExtensionType
	{
		// Instance specific
		World = 1 << 8,
		SpecularColor = 1 << 9,
		SpecularPower = 1 << 10
	};
} // namespace VertexComponent

//-------------------------------------------------------------------------

template<>
struct VertexElement<VertexElementType::World>
{
	math::Matrix4 world;

	constexpr VertexElement() = default;
	constexpr VertexElement(const math::Matrix4& world) : world(world) {}

	static constexpr auto Type = VertexElementType::World;
	static constexpr auto Description = MakeArray(
		InputElementDesc{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		InputElementDesc{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		InputElementDesc{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		InputElementDesc{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	);
};

//-------------------------------------------------------------------------

DECLARE_VERTEXINSTANCE_ELEMENT(VertexElementType::SpecularColor, math::Color, specularColor, SPECULARCOLOR)
DECLARE_VERTEXINSTANCE_ELEMENT(VertexElementType::SpecularPower, float, specularPower, SPECULARPOWER)

//-------------------------------------------------------------------------

extern template struct Vertex<VertexElementType::World, VertexElementType::SpecularColor, VertexElementType::SpecularPower>;

//-------------------------------------------------------------------------

using VertexInstanceSpecular = Vertex<VertexElementType::World, VertexElementType::SpecularColor, VertexElementType::SpecularPower>;

} // namespace library
