#include "StdAfx.h"
#include "library/Material.h"

#include "library/Model.h"
#include "library/Exception.h"
#include "library/Utils.h"

#include <assimp/scene.h>

namespace library
{
	namespace
	{
		const std::map<TextureType, aiTextureType> k_textureTypeMappings =
		{
			{ TextureType::Diffuse, aiTextureType_DIFFUSE },
			{ TextureType::SpecularMap, aiTextureType_SPECULAR },
			{ TextureType::Ambient, aiTextureType_AMBIENT },
			{ TextureType::Emissive, aiTextureType_EMISSIVE },
			{ TextureType::HeightMap, aiTextureType_HEIGHT },
			{ TextureType::NormalMap, aiTextureType_NORMALS },
			{ TextureType::SpecularPowerMap, aiTextureType_SHININESS },
			{ TextureType::DisplacementMap, aiTextureType_DISPLACEMENT },
			{ TextureType::LightMap, aiTextureType_LIGHTMAP },
		};
	}

	const TextureNamesVector& Material::GetTextureNames(const TextureType textureType) const
	{
		auto it = m_textures.find(textureType);
		assert(it != m_textures.end());
		return it->second;
	}

	Material::Material(Model& model, aiMaterial& material)
		: m_model(model)
	{
		aiString name;
		material.Get(AI_MATKEY_NAME, name);
		m_name = name.C_Str();

		for (const auto& tt : TextureType())
		{
			const auto aiTextureType = k_textureTypeMappings.at(tt);

			const auto texturesCount = material.GetTextureCount(aiTextureType);
			if (texturesCount > 0)
			{
				TextureNamesVector textureNames;
				textureNames.reserve(texturesCount);

				for (unsigned i = 0; i < texturesCount; i++)
				{
					aiString path;
					if (material.GetTexture(aiTextureType, i, &path) == AI_SUCCESS)
					{
						textureNames.emplace_back(utils::ToWideString(std::string(path.C_Str())));
					}
				}

				m_textures.emplace(tt, textureNames);
			}
		}
	}
} // namespace library
