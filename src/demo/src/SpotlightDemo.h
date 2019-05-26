#pragma once
#include <library/Materials/SpotlightMaterial.h>
#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
class SpotlightComponent;
class ProxyModelComponent;
class TextComponent;
} // namespace library

class SpotlightDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::SpotlightMaterial>
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

	void UpdateAmbientLight(const library::Time& time);
	void UpdateSpotlight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;

	std::unique_ptr<library::SpotlightComponent> m_spotlight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
