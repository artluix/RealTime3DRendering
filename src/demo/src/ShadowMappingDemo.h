#pragma once
#include <library/Materials/ShadowMappingMaterial.h>

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

#include <library/Frustum.h>
#include <library/math/Color.h>
#include <library/Math/Math.h>

#include <memory>
#include <optional>

namespace library
{
	class PointLightComponent;
	class PerspectiveProjectorComponent;
	class ProxyModelComponent;
	class RenderableFrustumComponent;
	class UIComponent;
	class TextComponent;

	class DepthMapRenderTarget;
} // namespace library

//-------------------------------------------------------------------------

class ShadowMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::ShadowMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ShadowMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit ShadowMappingDemo();
	~ShadowMappingDemo();

	void Initialize() override;
	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	void UpdateTechnique();

	void UpdateDepthBias(const library::Time& time);
	void UpdateDepthBiasState();

	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLightAndProjector(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	void InitializeProjectedTextureScalingMatrix();

	library::ShadowMappingTechnique::Type m_techniqueType;

	std::unique_ptr<library::TextComponent> m_text;
	std::unique_ptr<library::UIComponent> m_uiDepthMap;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;

	std::unique_ptr<library::PerspectiveProjectorComponent> m_projector;
	std::unique_ptr<library::RenderableFrustumComponent> m_renderableProjectorFrustum;
	library::Frustum m_projectorFrustum;
	library::math::Matrix4 m_projectedTextureScalingMatrix;

	// plane appendix to m_input data
	library::BufferData m_positionVertices; // for depth map

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
	library::ComPtr<ID3D11RasterizerState> m_depthBiasState;
	float m_depthBias;
	float m_slopeScaledDepthBias;

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;
};
