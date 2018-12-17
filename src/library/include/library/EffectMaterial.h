#pragma once
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"

#include <string>

namespace library
{
	class EffectMaterial
		: public rtti::Class<EffectMaterial>
		, public NonCopyable<EffectMaterial>
	{
	public:
		explicit EffectMaterial();
		explicit EffectMaterial(const std::string& defaultTechniqueName);

		virtual ~EffectMaterial();


	};

} // namespace library
