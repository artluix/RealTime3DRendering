#include "StdAfx.h"
#include "library/Model/ModelMaterial.h"

#include "library/Model/Model.h"
#include "library/Exception.h"

#include <cassert>

namespace library
{
	constexpr TextureType::Type TextureType::FromAiTextureType(const aiTextureType aiTt)
	{
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
			default:						return None;
		}
	}

	constexpr aiTextureType TextureType::ToAiTextureType(const Type t)
	{
		switch (t)
		{
			case Diffuse:			return aiTextureType_DIFFUSE;
			case SpecularMap:		return aiTextureType_SPECULAR;
			case Ambient:			return aiTextureType_AMBIENT;
			case Emissive:			return aiTextureType_EMISSIVE;
			case HeightMap:			return aiTextureType_HEIGHT;
			case NormalMap:			return aiTextureType_NORMALS;
			case SpecularPowerMap:	return aiTextureType_SHININESS;
			case LightMap:			return aiTextureType_LIGHTMAP;
			default:				return aiTextureType_NONE;
		}
	}

	constexpr std::array<TextureType::Type, TextureType::Count> TextureType::GetValues()
	{
		return std::array<TextureType::Type, TextureType::Count>
		{
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

	//-------------------------------------------------------------------------

	bool ModelMaterial::HasTextureNames(const TextureType::Type textureType) const
	{
		return m_textureNamesMap.find(textureType) != m_textureNamesMap.end();
	}

	const TextureNameVector& ModelMaterial::GetTextureNames(const TextureType::Type textureType) const
	{
		assert(HasTextureNames(textureType));
		return m_textureNamesMap.at(textureType);
	}

	ModelMaterial::ModelMaterial(const Model& model, const aiMaterial& aiMaterial)
		: m_model(model)
	{
		aiString name;
		aiMaterial.Get(AI_MATKEY_NAME, name);
		m_name = name.C_Str();

		for (const auto tt : TextureType::GetValues())
		{
			const auto aiTextureType = TextureType::ToAiTextureType(tt);

			const auto texturesCount = aiMaterial.GetTextureCount(aiTextureType);
			if (texturesCount > 0)
			{
				TextureNameVector textureNames;
				textureNames.reserve(texturesCount);

				for (unsigned i = 0; i < texturesCount; i++)
				{
					aiString path;
					if (aiMaterial.GetTexture(aiTextureType, i, &path) == AI_SUCCESS)
					{
						textureNames.emplace_back(std::string(path.C_Str()));
					}
				}

				m_textureNamesMap[tt] = textureNames;
			}
		}
	}

	ModelMaterial::~ModelMaterial() = default;
} // namespace library
