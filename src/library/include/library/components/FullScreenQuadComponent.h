#pragma once
#include "library/components/DrawableInputMaterialComponent.h"

#include <functional>

namespace library
{
	class EffectPass;

	class FullScreenQuadComponent : public DrawableInputMaterialComponent
	{
		RTTI_CLASS(FullScreenQuadComponent, DrawableInputMaterialComponent)

	public:
		using MaterialUpdateFunction = std::function<void()>;

		explicit FullScreenQuadComponent() = default;
		~FullScreenQuadComponent();

		void SetMaterial(Material& material, const std::string& techniqueName, const std::string& passName = "p0");
		void SetActiveTechnique(const std::string& techniqueName, const std::string& passName = "p0");
		void SetMaterialUpdateFunction(const MaterialUpdateFunction& func);

		void Initialize(const Application& app) override;
		using DrawableComponent::Draw;

		const Material& GetMaterial() const override { return *m_material; }

	protected:
		Material& GetMaterial() override { return *m_material; }

		void Draw_SetData() override;

	private:
		Material* m_material = nullptr;
		EffectPass* m_pass = nullptr;

		MaterialUpdateFunction m_materialUpdateFunction;
	};
} // namespace library
