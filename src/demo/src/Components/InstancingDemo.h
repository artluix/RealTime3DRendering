#pragma once
#include "Materials/InstancingMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.h>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
class TextComponent;
class PointLightComponent;
} // namespace library

class InstancingDemo
	: public library::ConcreteMaterialPrimitiveComponent<InstancingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(InstancingDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	InstancingDemo();

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

	void UpdateAmbientLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdatePointLight(const library::Time& time, const library::KeyboardComponent& keyboard);

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;

	library::VertexBufferData m_instanceVertexBuffer; // we use only one

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::TextComponent> m_text;
};
