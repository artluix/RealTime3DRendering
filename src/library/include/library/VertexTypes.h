#pragma once
#include <DirectXMath.h>

// layout will be passed to shader directly
// so use DirectX types only!

namespace library
{
	struct VertexPosition
	{
		DirectX::XMFLOAT4 position;

		explicit VertexPosition(const DirectX::XMFLOAT4& position)
			: position(position)
		{
		}
	};

	//-------------------------------------------------------------------------

	struct VertexPositionColor : public VertexPosition
	{
		DirectX::XMFLOAT4 color;

		explicit VertexPositionColor(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& color)
			: VertexPosition(position)
			, color(color)
		{
		}
	};

	//-------------------------------------------------------------------------

	struct VertexPositionTexture : public VertexPosition
	{
		DirectX::XMFLOAT2 textureCoordinates;

		explicit VertexPositionTexture(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates)
			: VertexPosition(position)
			, textureCoordinates(textureCoordinates)
		{
		}
	};

	//-------------------------------------------------------------------------

	using VertexPositionSize = VertexPositionTexture;

	//-------------------------------------------------------------------------

	struct VertexPositionNormal : public VertexPosition
	{
		DirectX::XMFLOAT3 normal;

		explicit VertexPositionNormal(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT3& normal)
			: VertexPosition(position)
			, normal(normal)
		{
		}
	};

	//-------------------------------------------------------------------------

	struct VertexPositionTextureNormal : public VertexPositionTexture
	{
		DirectX::XMFLOAT3 normal;

		explicit VertexPositionTextureNormal(
			const DirectX::XMFLOAT4& position,
			const DirectX::XMFLOAT2& textureCoordinates,
			const DirectX::XMFLOAT3& normal
		)
			: VertexPositionTexture(position, textureCoordinates)
			, normal(normal)
		{
		}
	};

	//-------------------------------------------------------------------------

	struct VertexPositionTextureNormalSkinned : public VertexPositionTextureNormal
	{
		DirectX::XMUINT4 boneIndices;
		DirectX::XMFLOAT4 boneWeights;

		explicit VertexPositionTextureNormalSkinned(
			const DirectX::XMFLOAT4& position,
			const DirectX::XMFLOAT2& textureCoordinates,
			const DirectX::XMFLOAT3& normal,
			const DirectX::XMUINT4& boneIndices,
			const DirectX::XMFLOAT4& boneWeights
		)
			: VertexPositionTextureNormal(position, textureCoordinates, normal)
			, boneIndices(boneIndices)
			, boneWeights(boneWeights)
		{
		}
	};
} // namespace library
