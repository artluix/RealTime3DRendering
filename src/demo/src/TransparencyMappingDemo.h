#pragma once
#include <library/Materials/TransparencyMappingMaterial.h>
#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
class PointLightComponent;
class ProxyModelComponent;
class TextComponent;
} // namespace library

class TransparencyMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::TransparencyMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(TransparencyMappingDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	TransparencyMappingDemo();
	~TransparencyMappingDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;
	void Draw_Render(const library::PrimitiveData& primitiveData) override;

private:
	struct Texture
	{
		enum Type : unsigned
		{
			Default = 0,
			TransparencyMap,

			//# Count
			Count
		};
	};

	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	library::ComPtr<ID3D11ShaderResourceView> m_transparencyMapTexture;

	library::Color m_ambientColor;
	library::Color m_specularColor;
	float m_specularPower;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
