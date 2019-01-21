#pragma once
#include <library/materials/BasicMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/MaterialComponent.h>
#include <library/components/InputReceivableComponent.h>

namespace demo
{
	class BasicComponent
		: public library::SceneComponent
		, public library::ConcreteMaterialComponent<library::BasicMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(BasicComponent, library::SceneComponent, library::MaterialComponent, library::InputReceivableComponent)

	public:
		explicit BasicComponent(const library::Application& app);

		void Initialize() override;
		void Update(const library::Time& time) override;
		using DrawableComponent::Draw;

	protected:
		void SetEffectData() override;
	};
} // namespace demo
