#pragma once
#include "library/components/MaterialComponent.h"

#include <functional>

namespace library
{
	class EffectPass;

	class FullScreenQuadComponent : public MaterialComponent
	{
		RTTI_CLASS(FullScreenQuadComponent, MaterialComponent)

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

		const Material& GetMaterial() const override { return *m_material; }

	protected:
		Material& GetMaterial() override { return *m_material; }

		void SetEffectData() override;

	private:
		Material* m_material = nullptr;
		EffectPass* m_pass = nullptr;

		MaterialUpdateFunction m_materialUpdateFunction;
	};
} // namespace library
