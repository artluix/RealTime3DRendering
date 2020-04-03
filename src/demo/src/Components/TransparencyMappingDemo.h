#pragma once
#include "Materials/TransparencyMappingMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.h>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Color.h>

namespace library
{
class PointLightComponent;
class TextComponent;
} // namespace library

class TransparencyMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<TransparencyMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(TransparencyMappingDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	TransparencyMappingDemo();
	~TransparencyMappingDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;
	void Draw_Render(const library::PrimitiveData& primitiveData) override;

private:
	struct Texture
	{
		enum Type : unsigned
		{
			Default = 0,
			TransparencyMap,

			//# Count
			Count
		};
	};

	void UpdateAmbientLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdatePointLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdateSpecularLight(const library::Time& time, const library::KeyboardComponent& keyboard);

	library::ComPtr<ID3D11ShaderResourceView> m_transparencyMapTexture;

	library::math::Color m_ambientColor;
	library::math::Color m_specularColor;
	float m_specularPower;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::TextComponent> m_text;
};
