#pragma once
#include <library/Materials/ProjectiveTextureMappingMaterial.h>

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

#include <library/Frustum.h>
#include <library/math/Color.h>
#include <library/Math/Math.h>

#include <library/DirectXForwardDeclarations.h>

#include <memory>

namespace library
{
	class PointLightComponent;
	class PerspectiveProjectorComponent;
	class ProxyModelComponent;
	class RenderableFrustumComponent;
	class TextComponent;
} // namespace library

class ProjectiveTextureMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::ProjectiveTextureMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ProjectiveTextureMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit ProjectiveTextureMappingDemo();
	~ProjectiveTextureMappingDemo();

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
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

	std::unique_ptr<library::PerspectiveProjectorComponent> m_projector;
	std::unique_ptr<library::RenderableFrustumComponent> m_renderableProjectorFrustum;
	library::Frustum m_projectorFrustum;

	library::math::Matrix4 m_projectedTextureScalingMatrix;

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;

	library::ComPtr<ID3D11ShaderResourceView> m_projectedTexture;
};
