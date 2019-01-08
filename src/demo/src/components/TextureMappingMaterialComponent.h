#pragma once
#include <library/components/SceneComponent.h>
#include <library/materials/TextureMappingMaterial.h>
#include <library/components/ConcreteMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>
#include <library/DirectXForwardDeclarations.h>

namespace demo
{
	class TextureMappingMaterialComponent
		: public library::SceneComponent
		, public library::ConcreteMaterialComponent<library::TextureMappingMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(TextureMappingMaterialComponent, library::SceneComponent, library::InputReceivableComponent, library::MaterialComponent)

	public:
		explicit TextureMappingMaterialComponent(const library::Application& app);

		void Initialize() override;
		void Update(const library::Time& time) override;

		using MaterialComponent::Draw;

	protected:
		void SetEffectData() override;

	private:
		ComPtr<ID3D11ShaderResourceView> m_textureShaderResourceView;
	};

} // namespace demo
