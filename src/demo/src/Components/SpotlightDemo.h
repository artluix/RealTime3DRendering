#pragma once
#include "Materials/LightsMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Color.h>

namespace library
{
class SpotlightComponent;
class ProxyModelComponent;
class TextComponent;
} // namespace library

class SpotlightDemo
	: public library::ConcreteMaterialPrimitiveComponent<LightsMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(SpotlightDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	SpotlightDemo();
	~SpotlightDemo();

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

			//# Count
			Count
		};
	};

	void UpdateAmbientLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdateSpotlight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdateSpecularLight(const library::Time& time, const library::KeyboardComponent& keyboard);

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;

	std::unique_ptr<library::SpotlightComponent> m_spotlight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
