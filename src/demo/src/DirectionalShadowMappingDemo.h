#pragma once
#include <library/Materials/DirectionalShadowMappingMaterial.h>

#include <library/Components/ConcreteMaterialSceneComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

#include <library/Frustum.h>
#include <library/Color.h>
#include <library/Math/Math.h>

#include <memory>
#include <optional>

namespace library
{
	class DirectionalLightComponent;
	class OrthographicProjectorComponent;
	class ProxyModelComponent;
	class RenderableFrustumComponent;
	class UIComponent;
	class TextComponent;

	class DepthMapMaterial;
	class DepthMapRenderTarget;
} // namespace library

//-------------------------------------------------------------------------

class DirectionalShadowMappingDemo
	: public library::ConcreteMaterialSceneComponent<library::DirectionalShadowMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(DirectionalShadowMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit DirectionalShadowMappingDemo();
	~DirectionalShadowMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

protected:
	void Draw_SetData() override;

private:
	void UpdateTechnique();

	void UpdateDepthBias(const library::Time& time);
	void UpdateDepthBiasState();

	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLightAndProjector(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	void InitializeProjectedTextureScalingMatrix();

	library::DirectionalShadowMappingTechnique::Type m_techniqueType;

	std::unique_ptr<library::TextComponent> m_text;
	std::unique_ptr<library::UIComponent> m_uiDepthMap;

	std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;

	std::unique_ptr<library::OrthographicProjectorComponent> m_projector;
	std::unique_ptr<library::RenderableFrustumComponent> m_renderableProjectorFrustum;
	library::Frustum m_projectorFrustum;
	library::math::Matrix4 m_projectedTextureScalingMatrix;

	// plane appendix to m_input data
	library::BufferData m_positionVertexBuffer; // for depth map

	// model data
	struct
	{
		library::BufferData vertexBuffer;
		std::optional<library::BufferData> indexBuffer;
		library::BufferData positionVertexBuffer; // for depth map
		library::math::Matrix4 worldMatrix;
	} m_model;

	bool m_drawDepthMap;
	std::shared_ptr<library::Effect> m_depthMapEffect;
	std::unique_ptr<library::DepthMapMaterial> m_depthMapMaterial;
	std::unique_ptr<library::DepthMapRenderTarget> m_depthMapRenderTarget;
	ComPtr<ID3D11RasterizerState> m_depthBiasState;
	float m_depthBias;
	float m_slopeScaledDepthBias;

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;
};
