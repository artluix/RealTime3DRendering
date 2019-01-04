#pragma once
#include "library/components/BaseComponent.h"
#include "library/lights/Light.h"

namespace library
{
	template<class LightType>
	class ConcreteLightComponent;
	
	// ----------------------------------------------------------------------------------------------------------

	template<>
	class ConcreteLightComponent<Light>
		: public rtti::Class<ConcreteLightComponent<Light>, BaseComponent>
		, public Light
	{
		explicit ConcreteLightComponent(const Application& app)
			: Class(app)
		{
		}

		~ConcreteLightComponent() = default;
	};

	using LightComponent = ConcreteLightComponent<Light>;

	// ----------------------------------------------------------------------------------------------------------
	// we inherited from LightComponent for other types of light
	// in order to know whether it's LightComponent or not :)
	// ----------------------------------------------------------------------------------------------------------

	template<class LightType>
	class ConcreteLightComponent
		: public rtti::Class<ConcreteLightComponent<LightType>, LightComponent>
		, public LightType
	{
		explicit ConcreteLightComponent(const Application& app)
			: Class(app)
		{
		}

		~ConcreteLightComponent() = default;
	};

} // namespace library
