#pragma once
#include <library/materials/BasicMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/ConcreteDrawableInputMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>

class BasicMaterialDemo
	: public library::SceneComponent
	, public library::ConcreteDrawableInputMaterialComponent<library::BasicMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(BasicMaterialDemo, library::SceneComponent, library::DrawableInputMaterialComponent, library::InputReceivableComponent)

public:
	explicit BasicMaterialDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData() override;
};
