#pragma once
#include <library/materials/BasicMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/ConcreteMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>

class BasicMaterialDemo
	: public library::SceneComponent
	, public library::ConcreteMaterialComponent<library::BasicMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(BasicMaterialDemo, library::SceneComponent, library::MaterialComponent, library::InputReceivableComponent)

public:
	explicit BasicMaterialDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;
	using DrawableComponent::Draw;

protected:
	void SetEffectData() override;
};
