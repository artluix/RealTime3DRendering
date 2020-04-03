#pragma once
#include "Materials/PhongLightsMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.h>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
class TextComponent;
class PointLightComponent;
} // namespace library

class MultipleLightsDemo
	: public library::ConcreteMaterialPrimitiveComponent<PhongLightsMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(MultipleLightsDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	MultipleLightsDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	// part from PointLightDemo
	struct Texture
	{
		enum Type : unsigned
		{
			Default = 0,

			//# Count
			Count
		};
	};

	//-------------------------------------------------------------------------

	void UpdateAmbientLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdatePointLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdateSpecularLight(const library::Time& time, const library::KeyboardComponent& keyboard);

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;

	library::DynArray<std::unique_ptr<library::PointLightComponent>> m_lights;
	std::unique_ptr<library::TextComponent> m_text;

	library::math::Color m_lightColor; // use common color

	unsigned m_lightsCount;
};
