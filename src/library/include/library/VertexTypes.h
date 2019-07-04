#pragma once
#include "library/Math/Color.h"

// layout will be passed to shader directly
// so use DirectX types only!

namespace library
{
struct Vertex
{};

struct VertexPosition : public Vertex
{
	math::Vector4 position;

	explicit constexpr VertexPosition(const math::Vector4& position) : position(position) {}
};

//-------------------------------------------------------------------------

struct VertexPositionColor : public VertexPosition
{
	math::Color color;

	constexpr VertexPositionColor(const math::Vector4& position, const math::Color& color)
		: VertexPosition(position)
		, color(color)
	{}
};

//-------------------------------------------------------------------------

struct VertexPositionTexture : public VertexPosition
{
	math::Vector2 textureCoordinates;

	constexpr VertexPositionTexture(const math::Vector4& position, const math::Vector2& textureCoordinates)
		: VertexPosition(position)
		, textureCoordinates(textureCoordinates)
	{}
};

//-------------------------------------------------------------------------

using VertexPositionSize = VertexPositionTexture;

//-------------------------------------------------------------------------

struct VertexPositionNormal : public VertexPosition
{
	math::Vector3 normal;

	constexpr VertexPositionNormal(const math::Vector4& position, const math::Vector3& normal)
		: VertexPosition(position)
		, normal(normal)
	{}
};

//-------------------------------------------------------------------------

struct VertexPositionTextureNormal : public VertexPositionTexture
{
	math::Vector3 normal;

	constexpr VertexPositionTextureNormal(
		const math::Vector4& position,
		const math::Vector2& textureCoordinates,
		const math::Vector3& normal
	)
		: VertexPositionTexture(position, textureCoordinates)
		, normal(normal)
	{}
};

//-------------------------------------------------------------------------

struct VertexPositionTextureNormalTangent : public VertexPositionTextureNormal
{
	math::Vector3 tangent;

	constexpr VertexPositionTextureNormalTangent(
		const math::Vector4& position,
		const math::Vector2& textureCoordinates,
		const math::Vector3& normal,
		const math::Vector3& tangent
	)
		: VertexPositionTextureNormal(position, textureCoordinates, normal)
		, tangent(tangent)
	{}
};

//-------------------------------------------------------------------------

struct VertexPositionTextureNormalSkinned : public VertexPositionTextureNormal
{
	math::Vector4u boneIndices;
	math::Vector4 boneWeights;

	constexpr VertexPositionTextureNormalSkinned(
		const math::Vector4& position,
		const math::Vector2& textureCoordinates,
		const math::Vector3& normal,
		const math::Vector4u& boneIndices,
		const math::Vector4& boneWeights
	)
		: VertexPositionTextureNormal(position, textureCoordinates, normal)
		, boneIndices(boneIndices)
		, boneWeights(boneWeights)
	{}
};
} // namespace library
