#pragma once
#include <library/materials/BasicMaterial.h>

#include <library/components/ConcreteMaterialSceneComponent.hpp>
#include <library/components/InputReceivableComponent.h>

class BasicMaterialDemo
	: public library::ConcreteMaterialSceneComponent<library::BasicMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(BasicMaterialDemo, library::MaterialSceneComponent, library::InputReceivableComponent)

public:
	explicit BasicMaterialDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData() override;
};
