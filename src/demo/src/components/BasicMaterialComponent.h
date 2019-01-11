#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/InputReceivableComponent.h>
#include <library/effect/materials/BasicEffectMaterial.h>

#include <memory>

namespace library
{
	class Effect;
} // namespace library

namespace demo
{
	class BasicMaterialComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(BasicMaterialComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit BasicMaterialComponent(const library::Application& app);

		void Initialize() override;
		void Update(const library::Time& time) override;
		using DrawableComponent::Draw;

	protected:
		void SetEffectData() override;

	private:
		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::BasicEffectMaterial> m_material;
	};
} // namespace demo
