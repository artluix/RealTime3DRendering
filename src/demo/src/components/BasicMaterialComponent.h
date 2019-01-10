#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/InputReceivableComponent.h>
#include <library/effect/materials/BasicMaterial.h>
#include <library/components/ConcreteMaterialComponent.hpp>

namespace demo
{
	class BasicMaterialComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
		, public library::ConcreteMaterialComponent<library::BasicMaterial>
	{
		RTTI_CLASS(BasicMaterialComponent, library::SceneComponent, library::InputReceivableComponent, library::MaterialComponent)

	public:
		explicit BasicMaterialComponent(const library::Application& app);

		void Initialize() override;
		void Update(const library::Time& time) override;
		using MaterialComponent::Draw;

	protected:
		void SetEffectData() override;
	};

} // namespace demo
