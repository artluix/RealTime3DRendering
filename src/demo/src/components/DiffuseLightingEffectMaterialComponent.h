#pragma once
#include <library/components/SceneComponent.h>
#include <library/effect/materials/DiffuseLightingEffectMaterial.h>
#include <library/components/InputReceivableComponent.h>
#include <library/Color.h>
#include <library/DirectXForwardDeclarations.h>

#include <memory>

namespace library
{
	class DirectionalLightComponent;
	class ProxyModelComponent;
} // namespace library

namespace demo
{
	class DiffuseLightingEffectMaterialComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(DiffuseLightingEffectMaterialComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit DiffuseLightingEffectMaterialComponent(const library::Application& app);
		~DiffuseLightingEffectMaterialComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		void UpdateAmbientLight(const library::Time& time);
		void UpdateDirectionalLight(const library::Time& time);

		ComPtr<ID3D11ShaderResourceView> m_textureShaderResourceView;

		library::Color m_ambientColor;
		
		std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	};
} // namespace demo
