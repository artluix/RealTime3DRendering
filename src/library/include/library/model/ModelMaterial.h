#pragma once
#include "library/NonCopyable.hpp"

#include <string>
#include <vector>
#include <map>

struct aiMaterial;

namespace library
{
	enum class TextureType
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

		First = Diffuse,
		Last = Count
	};

	// for range based loop

	inline TextureType operator++(TextureType& tt)
	{
		return tt = static_cast<TextureType>(std::underlying_type_t<TextureType>(tt) + 1);
	}

	inline TextureType operator * (const TextureType tt)
	{
		return tt;
	}

	inline TextureType begin(const TextureType tt)
	{
		return TextureType::First;
	}

	inline TextureType end(const TextureType tt)
	{
		return TextureType::Last;
	}

	//-------------------------------------------------------------------------

	using TextureNamesVector = std::vector<std::wstring>;

	class ModelMaterial : public NonCopyable<ModelMaterial>
	{
		friend class Model;

	public:
		~ModelMaterial();

		const Model& GetModel() const { return m_model; }
		const std::string& GetName() const { return m_name; }
		const std::map<TextureType, TextureNamesVector>& GetAllTextureNames() const { return m_textures; }

		const TextureNamesVector& GetTextureNames(const TextureType textureType) const;

	private:
		explicit ModelMaterial(const Model& model, const aiMaterial& material);

		const Model& m_model;
		std::string m_name;
		std::map<TextureType, TextureNamesVector> m_textures;
	};
} // namespace library
