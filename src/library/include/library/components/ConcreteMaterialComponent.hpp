#pragma once
#include "library/materials/Material.h"
#include "library/components/MaterialComponent.h"
#include "library/effect/Effect.h"

#include <type_traits>
#include <memory>

namespace library
{
	class Application;
	class Path;

	template<class MaterialType, typename = std::enable_if_t<std::is_base_of_v<Material, MaterialType>>>
	class ConcreteMaterialComponent : public virtual MaterialComponent
	{
	public:
		using Material = MaterialType;

		const Material& GetMaterial() const override { return *m_material; }

	protected:
		explicit ConcreteMaterialComponent() = default;

		void InitializeMaterial(const Application& app, const Path& effectPath, const bool compile = false)
		{
			m_effect = Effect::Create(app, effectPath);

			if (compile)
				m_effect->Compile();
			else
				m_effect->LoadCompiled();

			m_material = std::make_unique<Material>(*m_effect);
			m_material->Initialize();
		}

		Material& GetMaterial() override { return *m_material; }

		std::shared_ptr<Effect> m_effect;
		std::unique_ptr<Material> m_material;
	};
} // namespace library
