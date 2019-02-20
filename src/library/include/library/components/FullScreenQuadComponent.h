#pragma once
#include "library/components/MaterialSceneComponent.h"

#include <functional>

namespace library
{
	class EffectPass;

	class FullScreenQuadComponent : public MaterialSceneComponent
	{
		RTTI_CLASS(FullScreenQuadComponent, MaterialSceneComponent)

	public:
		using MaterialUpdateFunction = std::function<void()>;

		explicit FullScreenQuadComponent();
		~FullScreenQuadComponent();

		void SetMaterial(Material& material);
		void SetMaterial(Material& material, const std::string& techniqueName, const std::string& passName = "p0");
		
		void SetActiveTechnique(const std::string& techniqueName, const std::string& passName = "p0");
		void SetMaterialUpdateFunction(const MaterialUpdateFunction& func);

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;

		const Material& GetMaterial() const override { return *m_material; }

	protected:
		Material& GetMaterial() override { return *m_material; }

		void Draw_SetData() override;

		unsigned GetVertexSize() const override;

	private:
		Material* m_material = nullptr;
		EffectPass* m_pass = nullptr;

		MaterialUpdateFunction m_materialUpdateFunction;
	};
} // namespace library
