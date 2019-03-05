#pragma once
#include <library/Materials/ShadowMappingMaterial.h>

#include <library/Components/ConcreteMaterialSceneComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

#include <library/Frustum.h>
#include <library/Color.h>
#include <library/Math/Math.h>

#include <memory>
#include <optional>

namespace library
{
	class PointLightComponent;
	class ProjectorComponent;
	class ProxyModelComponent;
	class RenderableFrustumComponent;
	class UIComponent;
	class TextComponent;

	class DepthMapMaterial;
	class DepthMapRenderTarget;
} // namespace library

//-------------------------------------------------------------------------

struct DepthMappingTechnique
{
	enum Type : unsigned
	{
		Create = 0,
		//RenderTarget,
		Bias,

		//# Count
		Count
	};

	static constexpr Type Next(const Type t);
	static constexpr char* ToDisplayString(const Type t);
	static constexpr char* ToString(const Type t);
};

//-------------------------------------------------------------------------

class ShadowMappingDemo
	: public library::ConcreteMaterialSceneComponent<library::ShadowMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ShadowMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	struct Technique
	{
		enum Type : unsigned
		{
			Simple = 0,
			ManualPCF,
			PCF,

			//# Count
			Count
		};

		static constexpr DepthMappingTechnique::Type GetDepthMapTechniqueType(const Type t);
		static constexpr Type Next(const Type t);
		static constexpr char* ToDisplayString(const Type t);
		static constexpr char* ToString(const Type t);
	};

	//-------------------------------------------------------------------------

	explicit ShadowMappingDemo();
	~ShadowMappingDemo();

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

	Technique::Type m_techniqueType;

	std::unique_ptr<library::TextComponent> m_text;
	std::unique_ptr<library::UIComponent> m_uiDepthMap;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;

	std::unique_ptr<library::ProjectorComponent> m_projector;
	std::unique_ptr<library::RenderableFrustumComponent> m_renderableProjectorFrustum;
	library::Frustum m_projectorFrustum;
	library::math::Matrix4 m_projectedTextureScalingMatrix;

	// plane appendix to m_input data
	library::BufferData m_positionVertices; // for depth map

	// model data
	library::BufferData m_modelVertices;
	std::optional<library::BufferData> m_modelIndices;
	library::BufferData m_modelPositionVertices; // for depth map
	library::math::Matrix4 m_modelWorldMatrix;

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
