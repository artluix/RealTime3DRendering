#pragma once
#include "library/materials/PostProcessingMaterial.h"

#include "library/components/PostProcessingComponent.h"
#include "library/components/ConcreteMaterialComponent.hpp"

namespace library
{
	template<class MaterialType, typename = std::enable_if_t<std::is_base_of_v<PostProcessingMaterial, MaterialType>>>
	class ConcretePostProcessingComponent
		: public PostProcessingComponent
		, public ConcreteMaterialComponent<MaterialType>
	{
	public:
		using ConcreteMaterialComponent<MaterialType>::GetMaterial;

	protected:
		explicit ConcretePostProcessingComponent() = default;
	};
} // namespace library
