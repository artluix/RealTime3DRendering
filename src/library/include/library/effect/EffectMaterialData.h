#pragma once

namespace library
{
	class EffectPass;
	class Material;

	struct EffectMaterialData
	{
		Material* material;
		EffectPass* pass;
	};
}
// namespace library
