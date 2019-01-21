#pragma once
#include "library/components/DrawableComponent.h"

#include <functional>

namespace library
{
	class EffectPass;

	class FullScreenQuadComponent : public DrawableComponent
	{
		RTTI_CLASS(FullScreenQuadComponent, DrawableComponent)

	public:
		using MaterialUpdateFunction = std::function<void()>;

		explicit FullScreenQuadComponent(const Application& app);
		~FullScreenQuadComponent();

		void SetMaterial(
			Material& material,
			const std::string& techniqueName,
			const std::string& passName
		);

		void SetActiveTechnique(const std::string& techniqueName, const std::string& passName);
		void SetMaterialUpdateFunction(const MaterialUpdateFunction& func);

		void Initialize() override;
		using DrawableComponent::Draw;

	protected:
		void SetEffectData() override;

	private:
		Material* m_material = nullptr;
		EffectPass* m_pass = nullptr;

		MaterialUpdateFunction m_materialUpdateFunction;
	};
} // namespace library
