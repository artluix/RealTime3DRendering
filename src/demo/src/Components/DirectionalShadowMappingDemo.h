#pragma once
#include "Materials/DirectionalShadowMappingMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Math.h>

namespace library
{
class DirectionalLightComponent;
class ProjectorComponent;
class ProxyModelComponent;
class RenderableFrustumComponent;
class UIComponent;
class TextComponent;

class DepthMapRenderTarget;
} // namespace library

//-------------------------------------------------------------------------

class DirectionalShadowMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<DirectionalShadowMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(DirectionalShadowMappingDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	DirectionalShadowMappingDemo();
	~DirectionalShadowMappingDemo();

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

	void Draw_SetData(const library::math::Matrix4& worldMatrix, const library::PrimitiveData& primitiveData);

	void UpdateTechnique();

	void UpdateDepthBias(const library::Time& time);
	void UpdateDepthBiasState();

	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLightAndProjector(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	void InitializeProjectedTextureScalingMatrix();

	DirectionalShadowMappingTechnique::Type m_techniqueType;

	std::unique_ptr<library::TextComponent> m_text;
	std::unique_ptr<library::UIComponent> m_uiDepthMap;

	std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;

	std::unique_ptr<library::ProjectorComponent> m_projector;
	std::unique_ptr<library::RenderableFrustumComponent> m_renderableProjectorFrustum;
	library::math::Frustum m_projectorFrustum;
	library::math::Matrix4 m_projectedTextureScalingMatrix;

	// model data
	library::VertexBufferData m_modelPositionVertexBuffer;
	library::math::Matrix4 m_modelWorldMatrix = library::math::Matrix4::Identity;

	bool m_drawDepthMap;
	std::shared_ptr<library::Effect> m_depthMapEffect;
	std::unique_ptr<DepthMapMaterial> m_depthMapMaterial;
	std::unique_ptr<library::DepthMapRenderTarget> m_depthMapRenderTarget;
	library::ComPtr<ID3D11RasterizerState> m_depthBiasState;
	float m_depthBias;
	float m_slopeScaledDepthBias;

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;
};