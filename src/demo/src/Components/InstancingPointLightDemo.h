#pragma once
#include "Materials/InstancingPointLightMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
class TextComponent;
class PointLightComponent;
class ProxyModelComponent;
} // namespace library

class InstancingPointLightDemo
	: public library::ConcreteMaterialPrimitiveComponent<InstancingPointLightMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(InstancingPointLightDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	InstancingPointLightDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;

	void Draw_SetIA(const library::PrimitiveData& primitiveData) override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;
	void Draw_Render(const library::PrimitiveData& primitiveData) override;

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

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;

	library::VertexBufferData m_instanceVertexBuffer; // we use only one

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
