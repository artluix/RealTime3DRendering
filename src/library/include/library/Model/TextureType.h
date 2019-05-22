#pragma once
#include <assimp/material.h>
#include <array>

namespace library
{
struct TextureType
{
	enum Type : unsigned
	{
		Diffuse = 0,
		SpecularMap,
		Ambient,
		Emissive,
		HeightMap,
		NormalMap,
		SpecularPowerMap,
		DisplacementMap,
		LightMap,

		//# Count
		Count,
		None = Count
	};

	//-------------------------------------------------------------------------

	static constexpr Type FromAiTextureType(const aiTextureType aiTt)
	{
		// clang-format off
		switch (aiTt)
		{
			case aiTextureType_DIFFUSE:		return Diffuse;
			case aiTextureType_SPECULAR:	return SpecularMap;
			case aiTextureType_AMBIENT:		return Ambient;
			case aiTextureType_EMISSIVE:	return Emissive;
			case aiTextureType_HEIGHT:		return HeightMap;
			case aiTextureType_NORMALS:		return NormalMap;
			case aiTextureType_SHININESS:	return SpecularPowerMap;
			case aiTextureType_LIGHTMAP:	return LightMap;
			default: return None;
		}
		// clang-format on
	}

	static constexpr aiTextureType ToAiTextureType(const Type t)
	{
		// clang-format off
		switch (t)
		{
			case Diffuse:					return aiTextureType_DIFFUSE;
			case SpecularMap:				return aiTextureType_SPECULAR;
			case Ambient:					return aiTextureType_AMBIENT;
			case Emissive:					return aiTextureType_EMISSIVE;
			case HeightMap:					return aiTextureType_HEIGHT;
			case NormalMap:					return aiTextureType_NORMALS;
			case SpecularPowerMap:			return aiTextureType_SHININESS;
			case LightMap:					return aiTextureType_LIGHTMAP;
			default:						return aiTextureType_NONE;
		}
		// clang-format on
	}

	static constexpr std::array<Type, Count> GetValues()
	{
		return std::array<TextureType::Type, TextureType::Count>{
			Diffuse,
			SpecularMap,
			Ambient,
			Emissive,
			HeightMap,
			NormalMap,
			SpecularPowerMap,
			DisplacementMap,
			LightMap,
		};
	}
};
} // namespace library