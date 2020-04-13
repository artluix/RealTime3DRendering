#pragma once
#include "Materials/PbrLightsMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.h>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Color.h>

namespace library
{
class PointLightComponent;
class TextComponent;
} // namespace library

class PbrDemo
	: public library::ConcreteMaterialPrimitiveComponent<PbrLightsMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(PbrDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	PbrDemo();
	~PbrDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	struct Texture
	{
		enum Type : unsigned
		{
			Albedo = 0,
			Metallic,
			Roughness,
			AO,

			//# Count
			Count
		};
	};

	void UpdateAmbientLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdatePointLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdatePbrParameters(const library::Time& time, const library::KeyboardComponent& keyboard);

	library::math::Color m_ambientColor;

	library::math::Color m_albedo;
	float m_metallic;
	float m_roughness;
	float m_ao;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::TextComponent> m_text;
};
