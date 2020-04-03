#pragma once
#include "Materials/ProjectiveTextureMappingMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.h>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Frustum.h>
#include <library/Math/Math.h>

namespace library
{
class PointLightComponent;
class ProjectorComponent;
class RenderableFrustumComponent;
class TextComponent;
} // namespace library

class ProjectiveTextureMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<ProjectiveTextureMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ProjectiveTextureMappingDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	ProjectiveTextureMappingDemo();
	~ProjectiveTextureMappingDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	struct Texture
	{
		enum Type : unsigned
		{
			Default = 0,
			Projected,

			//# Count
			Count
		};
	};

	void UpdateAmbientLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdatePointLightAndProjector(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdateSpecularLight(const library::Time& time, const library::KeyboardComponent& keyboard);

	void InitializeProjectedTextureScalingMatrix(
		const unsigned textureWidth,
		const unsigned textureHeight
	);

	std::unique_ptr<library::TextComponent> m_text;

	std::unique_ptr<library::PointLightComponent> m_pointLight;

	std::unique_ptr<library::ProjectorComponent> m_projector;
	std::unique_ptr<library::RenderableFrustumComponent> m_renderableProjectorFrustum;
	library::math::Frustum m_projectorFrustum;

	library::math::Matrix4 m_projectedTextureScalingMatrix;

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;
};
