#pragma once
#include <library/Materials/EnvironmentMappingMaterial.h>

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>
#include <library/math/Color.h>

#include <memory>

namespace library
{
	class TextComponent;
} // namespace library

class EnvironmentMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::EnvironmentMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(EnvironmentMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit EnvironmentMappingDemo();
	~EnvironmentMappingDemo();

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	library::Color m_ambientColor;
	float m_reflectionAmount;

	library::ComPtr<ID3D11ShaderResourceView> m_environmentMapTexture;

	std::unique_ptr<library::TextComponent> m_text;
};
