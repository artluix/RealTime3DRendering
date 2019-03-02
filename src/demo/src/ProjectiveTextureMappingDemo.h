#pragma once
#include <library/materials/ProjectiveTextureMappingMaterial.h>

#include <library/components/ConcreteMaterialSceneComponent.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/Frustum.h>
#include <library/Color.h>
#include <library/math/Math.h>

#include <memory>

namespace library
{
	class PointLightComponent;
	class ProjectorComponent;
	class ProxyModelComponent;
	class RenderableFrustumComponent;
	class TextComponent;
} // namespace library

class ProjectiveTextureMappingDemo
	: public library::ConcreteMaterialSceneComponent<library::ProjectiveTextureMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ProjectiveTextureMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit ProjectiveTextureMappingDemo();
	~ProjectiveTextureMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData() override;

private:
	void UpdateTechnique();
	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLightAndProjector(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	void InitializeProjectedTextureScalingMatrix(
		const unsigned textureWidth,
		const unsigned textureHeight
	);

	std::unique_ptr<library::TextComponent> m_text;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::ProjectorComponent> m_projector;
	std::unique_ptr<library::RenderableFrustumComponent> m_renderableProjectorFrustum;
	library::Frustum m_projectorFrustum;

	library::math::Matrix4 m_projectedTextureScalingMatrix;

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;

	ComPtr<ID3D11ShaderResourceView> m_projectedTexture;
};
