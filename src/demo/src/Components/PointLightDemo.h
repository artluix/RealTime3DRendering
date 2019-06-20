#pragma once
#include "Materials/PointLightMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Color.h>

namespace library
{
class PointLightComponent;
class ProxyModelComponent;
class TextComponent;
} // namespace library

class PointLightDemo
	: public library::ConcreteMaterialPrimitiveComponent<PointLightMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(PointLightDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	PointLightDemo();
	~PointLightDemo();

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
	void UpdatePointLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
