#pragma once
#include <library/materials/TextureMappingMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/ConcreteMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>

class TextureMappingDemo
	: public library::SceneComponent
	, public library::ConcreteMaterialComponent<library::TextureMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(TextureMappingDemo, library::SceneComponent, library::MaterialComponent, library::InputReceivableComponent)

public:
	explicit TextureMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;
	using DrawableComponent::Draw;

protected:
	void SetEffectData() override;
};
