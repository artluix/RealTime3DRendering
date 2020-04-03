#pragma once
#include "library/Common.h"
#include "library/Model/TextureType.h"

#include <string>
#include <map>

namespace library
{
using TextureNameVector = DynArray<std::string>;
using TextureNamesMap = std::map<TextureType::Type, TextureNameVector>;

class ModelMaterial : public NonCopyable<ModelMaterial>
{
	friend class Model;

public:
	~ModelMaterial();

	const Model& GetModel() const { return m_model; }
	const std::string& GetName() const { return m_name; }

	bool HasTextureNames(const TextureType::Type textureType) const;
	const TextureNameVector& GetTextureNames(const TextureType::Type textureType) const;

private:
	ModelMaterial(const Model& model, const aiMaterial& material);

	const Model& m_model;
	std::string m_name;
	TextureNamesMap m_textureNamesMap;
};
} // namespace library
