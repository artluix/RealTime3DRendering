#pragma once
#include "library/Components/ProjectorComponent.h"

namespace library
{
class CameraComponent : public ProjectorComponent
{
	RTTI_CLASS(CameraComponent, ProjectorComponent)

public:
	using ProjectorComponent::ProjectorComponent;
	CameraComponent();

	void Initialize() override;
};
} // namespace library
