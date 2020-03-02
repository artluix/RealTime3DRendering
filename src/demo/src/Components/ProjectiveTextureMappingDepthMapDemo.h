#pragma once
#include "Materials/ProjectiveTextureMappingMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Math.h>

namespace library
{
class PointLightComponent;
class ProjectorComponent;
class RenderableFrustumComponent;
class UIComponent;
class TextComponent;

class DepthMapRenderTarget;
} // namespace library

class DepthMapMaterial;

class ProjectiveTextureMappingDepthMapDemo
	: public library::ConcreteMaterialPrimitiveComponent<ProjectiveTextureMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ProjectiveTextureMappingDepthMapDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	ProjectiveTextureMappingDepthMapDemo();
	~ProjectiveTextureMappingDepthMapDemo();

	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

protected:
	void InitializeInternal() override;

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

	struct Primitive
	{
		enum Type : unsigned
		{
			Plane = 0,
			Model,

			//# Count
			Count
		};
	};

	//-------------------------------------------------------------------------

	void Draw_SetData(const library::math::Matrix4& worldMatrix, const library::PrimitiveData& primitiveData);

	void UpdateDepthBias(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdateAmbientLight(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdatePointLightAndProjector(const library::Time& time, const library::KeyboardComponent& keyboard);
	void UpdateSpecularLight(const library::Time& time, const library::KeyboardComponent& keyboard);

	void InitializeProjectedTextureScalingMatrix();

	std::unique_ptr<library::TextComponent> m_text;
	std::unique_ptr<library::UIComponent> m_uiDepthMap;

	std::unique_ptr<library::PointLightComponent> m_pointLight;

	std::unique_ptr<library::ProjectorComponent> m_projector;
	std::unique_ptr<library::RenderableFrustumComponent> m_renderableProjectorFrustum;
	library::math::Frustum m_projectorFrustum;
	library::math::Matrix4 m_projectedTextureScalingMatrix;

	// model data
	library::VertexBufferData m_modelPositionVertexBuffer;
	library::math::Matrix4 m_modelWorldMatrix = library::math::Matrix4::Identity;

	bool m_drawDepthMap;
	float m_depthBias;
	std::shared_ptr<library::Effect> m_depthMapEffect;
	std::unique_ptr<DepthMapMaterial> m_depthMapMaterial;
	std::unique_ptr<library::DepthMapRenderTarget> m_depthMapRenderTarget;

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;
};
