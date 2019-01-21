#pragma once
#include <library/materials/TextureMappingMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/MaterialComponent.h>
#include <library/components/InputReceivableComponent.h>

namespace demo
{
	class TextureMappingComponent
		: public library::SceneComponent
		, public library::ConcreteMaterialComponent<library::TextureMappingMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(TextureMappingComponent, library::SceneComponent, library::MaterialComponent, library::InputReceivableComponent)

	public:
		explicit TextureMappingComponent(const library::Application& app);

		void Initialize() override;
		void Update(const library::Time& time) override;
		using DrawableComponent::Draw;

	protected:
		void SetEffectData() override;
	};
} // namespace demo
