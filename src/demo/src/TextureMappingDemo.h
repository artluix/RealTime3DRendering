#pragma once
#include <library/Materials/TextureMappingMaterial.h>

#include <library/Components/ConcreteMaterialSceneComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

class TextureMappingDemo
	: public library::ConcreteMaterialSceneComponent<library::TextureMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(TextureMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit TextureMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData() override;
};
