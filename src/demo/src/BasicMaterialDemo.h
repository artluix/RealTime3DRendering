#pragma once
#include <library/Materials/BasicMaterial.h>

#include <library/Components/ConcreteMaterialSceneComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

class BasicMaterialDemo
	: public library::ConcreteMaterialSceneComponent<library::BasicMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(BasicMaterialDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit BasicMaterialDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::MeshData& meshData) override;
};
