#pragma once
#include "Materials/NormalMappingMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Color.h>

namespace library
{
class DirectionalLightComponent;
class ProxyModelComponent;
class TextComponent;
} // namespace library

class NormalMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<NormalMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(NormalMappingDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	NormalMappingDemo();
	~NormalMappingDemo();

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
			NormalMap,

			//# Count
			Count
		};
	};

	void UpdateAmbientLight(const library::Time& time);
	void UpdateDirectionalLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;

	LightsData m_lightsData;

	std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
