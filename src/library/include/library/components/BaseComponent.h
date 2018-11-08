#pragma once
#include "library/NonCopyable.hpp"
#include "library/RTTI.hpp"
#include "library/Common.h"

namespace library
{
	class Application;
	struct Time;

	namespace components
	{
		class BaseComponent
			: public NonCopyable<BaseComponent>
			, public rtti::Class<BaseComponent>
		{
		public:
			explicit BaseComponent(const Application& app);
			virtual ~BaseComponent() = default;

			bool IsEnabled() const { return m_enabled; }
			void SetEnabled(const bool enabled);

			virtual void Initialize();
			virtual void Update(const Time& time);

		protected:
			const Application& m_app;
			bool m_enabled;
		};

		using BaseComponentPtr = std::shared_ptr<BaseComponent>;

	} // namespace components
} // namespace library
