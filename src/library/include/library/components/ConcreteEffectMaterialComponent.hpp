#pragma once
#include "library/components/EffectMaterialComponent.h"
#include "library/effect/EffectMaterial.h"

#include <type_traits>
#include <memory>

namespace library
{
	namespace effect
	{
		class Effect;
	} // namespace effect

	namespace components
	{
		template <class EffectMaterialType, typename = std::enable_if_t<std::is_base_of_v<effect::Material, EffectMaterialType>>>
		class ConcreteEffectMaterial : public EffectMaterial
		{
		public:
			using MaterialType = EffectMaterialType;

			const MaterialType& GetMaterial() const { return *m_material; }

		protected:
			using EffectMaterial::EffectMaterial;

			MaterialType& GetMaterial() { return *m_material; }

			std::unique_ptr<MaterialType> m_material;
			std::shared_ptr<effect::Effect> m_effect;
		};
	} // namespace components
} // namespace library
