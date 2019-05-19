#pragma once
#include <library/Materials/NormalMappingMaterial.h>

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

#include <library/math/Color.h>
#include <library/DirectXForwardDeclarations.h>

#include <memory>

namespace library
{
	class DirectionalLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

class NormalMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::NormalMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(NormalMappingDemo, library::SceneComponent , library::InputReceivableComponent)

public:
	explicit NormalMappingDemo();
	~NormalMappingDemo();

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	void UpdateAmbientLight(const library::Time& time);
	void UpdateDirectionalLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;

	library::ComPtr<ID3D11ShaderResourceView> m_normalMapTexture;

	std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
