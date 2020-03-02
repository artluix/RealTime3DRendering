#pragma once
#include "Materials/FogMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Color.h>

namespace library
{
class DirectionalLightComponent;
class ProxyModelComponent;
class TextComponent;
} // namespace library

class FogDemo
	: public library::ConcreteMaterialPrimitiveComponent<FogMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(FogDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	FogDemo();
	~FogDemo();

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
	void UpdateDirectionalLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void SetActiveTechnique();

	library::math::Color m_ambientColor;

	float m_fogStart;
	float m_fogRange;
	float m_fogEnabled;

	std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
