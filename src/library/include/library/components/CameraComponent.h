#pragma once
#include "library/Components/PerspectiveProjectorComponent.h"

namespace library
{
	class CameraComponent : public PerspectiveProjectorComponent
	{
		RTTI_CLASS(CameraComponent, Component)

	public:
		explicit CameraComponent();
		using PerspectiveProjectorComponent::PerspectiveProjectorComponent;

		void Initialize(const Application& app) override;

		void Reset() override;
	};
} // namespace library
