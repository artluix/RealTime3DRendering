#pragma once
#include "library/NonCopyable.hpp"

#include <string>
#include <vector>
#include <array>
#include <map>

#include <assimp/material.h>

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

		static constexpr Type FromAiTextureType(const aiTextureType aiTt);
		static constexpr aiTextureType ToAiTextureType(const Type t);
		static constexpr std::array<Type, Count> GetValues();
	};

	//-------------------------------------------------------------------------

	using TextureNameVector = std::vector<std::string>;
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
		explicit ModelMaterial(const Model& model, const aiMaterial& material);

		const Model& m_model;
		std::string m_name;
		TextureNamesMap m_textureNamesMap;
	};
} // namespace library
