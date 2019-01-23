#pragma once
#include "library/NonCopyable.hpp"

namespace library
{
	class Material;

	class IMaterialComponent : public NonCopyable<IMaterialComponent>
	{
	public:
		virtual ~IMaterialComponent() = default;
		virtual const Material& GetMaterial() const = 0;

	protected:
		explicit IMaterialComponent() = default;
		virtual Material& GetMaterial() = 0;
	};
} // namespace library
