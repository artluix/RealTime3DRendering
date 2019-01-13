#include "StdAfx.h"
#include "library/ModelMaterial.h"

#include "library/Model.h"
#include "library/Exception.h"
#include "library/Utils.h"

namespace library
{
	constexpr TextureType::Type TextureType::FromAiTextureType(const aiTextureType aiTt)
	{
		switch (aiTt)
		{
			case aiTextureType_DIFFUSE: return Diffuse;
			case aiTextureType_SPECULAR: return SpecularMap;
			case aiTextureType_AMBIENT: return Ambient;
			case aiTextureType_EMISSIVE: return Emissive;
			case aiTextureType_HEIGHT: return HeightMap;
			case aiTextureType_NORMALS: return NormalMap;
			case aiTextureType_SHININESS: return SpecularPowerMap;
			case aiTextureType_LIGHTMAP: return LightMap;

			default: return None;
		}
	}

	constexpr aiTextureType TextureType::ToAiTextureType(const Type t)
	{
		switch (t)
		{
			case Diffuse: return aiTextureType_DIFFUSE;
			case SpecularMap: return aiTextureType_SPECULAR;
			case Ambient: return aiTextureType_AMBIENT;
			case Emissive: return aiTextureType_EMISSIVE;
			case HeightMap: return aiTextureType_HEIGHT;
			case NormalMap: return aiTextureType_NORMALS;
			case SpecularPowerMap: return aiTextureType_SHININESS;
			case LightMap: return aiTextureType_LIGHTMAP;

			default: return aiTextureType_NONE;
		}
	}

	constexpr std::array<TextureType::Type, TextureType::Count> TextureType::Values()
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

	const TextureNameVector& ModelMaterial::GetTextureNames(const TextureType::Type textureType) const
	{
		return m_textures[textureType];
	}

	ModelMaterial::ModelMaterial(const Model& model, const aiMaterial& material)
		: m_model(model)
	{
		aiString name;
		material.Get(AI_MATKEY_NAME, name);
		m_name = name.C_Str();

		for (const auto tt : TextureType::Values())
		{
			const auto aiTextureType = TextureType::ToAiTextureType(tt);

			const auto texturesCount = material.GetTextureCount(aiTextureType);
			if (texturesCount > 0)
			{
				TextureNameVector textureNames;
				textureNames.reserve(texturesCount);

				for (unsigned i = 0; i < texturesCount; i++)
				{
					aiString path;
					if (material.GetTexture(aiTextureType, i, &path) == AI_SUCCESS)
					{
						textureNames.emplace_back(utils::ToWideString(std::string(path.C_Str())));
					}
				}

				m_textures[tt] = textureNames;
			}
		}
	}

	ModelMaterial::~ModelMaterial() = default;
} // namespace library
