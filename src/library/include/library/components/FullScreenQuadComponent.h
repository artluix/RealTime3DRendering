#pragma once
#include "library/Components/SceneComponent.h"

#include <functional>

namespace library
{
	class EffectPass;

	class FullScreenQuadComponent : public SceneComponent
	{
		RTTI_CLASS(FullScreenQuadComponent, SceneComponent)

	public:
		using MaterialUpdateFunction = std::function<void()>;

		explicit FullScreenQuadComponent();
		~FullScreenQuadComponent();

		void SetMaterial(Material& material);
		void SetMaterial(
			Material& material,
			const std::string& techniqueName,
			const std::string& passName = "p0"
		);
		
		void SetActiveTechnique(const std::string& techniqueName, const std::string& passName = "p0");
		void SetMaterialUpdateFunction(const MaterialUpdateFunction& func);

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;

		const Material* GetMaterial() const override { return m_material; }

	protected:
		void Draw_SetData(const MeshData& meshData) override;

		Material* GetMaterial() override { return m_material; }
		unsigned GetVertexSize() const override;

		MaterialUpdateFunction m_materialUpdateFunction;
		Material* m_material = nullptr;

	private:
		EffectPass* m_pass = nullptr;
	};
} // namespace library
