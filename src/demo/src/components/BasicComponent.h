#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/InputReceivableComponent.h>
#include <library/materials/BasicMaterial.h>

#include <memory>

namespace library
{
	class Effect;
} // namespace library

namespace demo
{
	class BasicComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(BasicComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit BasicComponent(const library::Application& app);

		void Initialize() override;
		void Update(const library::Time& time) override;
		using DrawableComponent::Draw;

	protected:
		void SetEffectData() override;

	private:
		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::BasicMaterial> m_material;
	};
} // namespace demo
