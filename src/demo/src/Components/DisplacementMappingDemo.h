#pragma once
#include "Materials/DisplacementMappingMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Color.h>

namespace library
{
class PointLightComponent;
class TextComponent;
} // namespace library

class DisplacementMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<DisplacementMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(DisplacementMappingDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	DisplacementMappingDemo();
	~DisplacementMappingDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	struct Texture
	{
		enum Type : unsigned
		{
			Default = 0,
			DisplacementMap,

			//# Count
			Count
		};
	};

	void UpdateAmbientLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdatePointLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdateDisplacement(const library::Time& time, const library::KeyboardComponent& keyboard);

	library::ComPtr<ID3D11ShaderResourceView> m_displacementMapTexture;

	library::math::Color m_ambientColor;
	library::math::Color m_specularColor;
	float m_specularPower;

	float m_displacementScale;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::TextComponent> m_text;
};
