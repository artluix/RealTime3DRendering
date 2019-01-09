#pragma once
#include <library/components/SceneComponent.h>
#include <library/materials/DiffuseLightingMaterial.h>
#include <library/components/ConcreteMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/lights/DirectionalLight.h>

#include <library/DirectXForwardDeclarations.h>

namespace demo
{
	class DiffuseLightingMaterialComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
		, public library::ConcreteMaterialComponent<library::DiffuseLightingMaterial>
	{
		RTTI_CLASS(DiffuseLightingMaterialComponent, library::SceneComponent, library::InputReceivableComponent, library::MaterialComponent)

	public:
		explicit DiffuseLightingMaterialComponent(const library::Application& app);

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		void UpdateAmbientLight(const library::Time& time);
		void UpdateDirectionalLight(const library::Time& time);

		ComPtr<ID3D11ShaderResourceView> m_textureShaderResourceView;

		library::Color m_ambientColor;
		DirectionalLight m_directionalLight;

		ProxyModelCom
	};
} // namespace demo
