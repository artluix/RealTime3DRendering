#pragma once
#include <library/materials/FogMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/ConcreteMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/DirectXForwardDeclarations.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class DirectionalLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

namespace demo
{
	class FogComponent
		: public library::SceneComponent
		, public library::ConcreteMaterialComponent<library::FogMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(FogComponent, library::SceneComponent, library::MaterialComponent, library::InputReceivableComponent)

	public:
		explicit FogComponent();
		~FogComponent();

		void Initialize(const library::Application& app) override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

	private:
		void UpdateAmbientLight(const library::Time& time);
		void UpdateDirectionalLight(const library::Time& time);
		void SetActiveTechnique();

		void SetEffectData() override;

		library::Color m_ambientColor;

		float m_fogStart;
		float m_fogRange;
		float m_fogEnabled;

		std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
