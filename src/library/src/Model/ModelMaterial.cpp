#include "StdAfx.h"
#include "library/Model/ModelMaterial.h"

#include "library/Model/Model.h"
#include "library/Exception.h"

#include <cassert>

namespace library
{
ModelMaterial::ModelMaterial(const Model& model, const aiMaterial& aiMaterial) : m_model(model)
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
} // namespace library
