#pragma once
#include <library/Materials/EnvironmentMappingMaterial.h>

#include <library/Components/ConcreteMaterialSceneComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

#include <library/DirectXForwardDeclarations.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class TextComponent;
} // namespace library

class EnvironmentMappingDemo
	: public library::ConcreteMaterialSceneComponent<library::EnvironmentMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(EnvironmentMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit EnvironmentMappingDemo();
	~EnvironmentMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

private:
	void Draw_SetData() override;

	library::Color m_ambientColor;
	float m_reflectionAmount;

	ComPtr<ID3D11ShaderResourceView> m_environmentMapTexture;

	std::unique_ptr<library::TextComponent> m_text;
};
