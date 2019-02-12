#pragma once
#include "library/NonCopyable.hpp"

namespace library
{
	struct Time;

	class IComponent : public NonCopyable<IComponent>
	{
	public:
		virtual ~IComponent() = default;

		virtual void Initialize() = 0;
		virtual void Update(const Time& time) = 0;

	protected:
		explicit IComponent() = default;
	};
} // namespace library
