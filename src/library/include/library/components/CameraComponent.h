#pragma once
#include "library/components/ProjectorComponent.h"

namespace library
{
	class CameraComponent : public ProjectorComponent
	{
		RTTI_CLASS(CameraComponent, Component)

	public:
		explicit CameraComponent();
		using ProjectorComponent::ProjectorComponent;

		void Initialize(const Application& app) override;

		void Reset() override;
	};
} // namespace library
