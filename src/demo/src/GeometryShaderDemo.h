#pragma once
#include <library/Materials/PointSpriteMaterial.h>
#include <library/Components/ConcreteMaterialSceneComponent.hpp>

#include <library/Components/InputReceivableComponent.h>

namespace library
{
	class TextComponent;
} // namespace library

class GeometryShaderDemo
	: public library::ConcreteMaterialSceneComponent<library::PointSpriteMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(GeometryShaderDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit GeometryShaderDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::MeshData& meshData) override;
	void Draw_Render(const library::MeshData& meshData) override;

private:
	void InitializeRandomPoints();
	void InitializeFixedPoints();

	std::unique_ptr<library::TextComponent> m_text;

	bool m_showRandomPoints;
};
