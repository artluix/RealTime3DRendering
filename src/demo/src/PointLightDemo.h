#pragma once
#include <library/Materials/PointLightMaterial.h>

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

#include <library/math/Color.h>

#include <memory>

namespace library
{
	class PointLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

class PointLightDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::PointLightMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(PointLightDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit PointLightDemo();
	~PointLightDemo();

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
