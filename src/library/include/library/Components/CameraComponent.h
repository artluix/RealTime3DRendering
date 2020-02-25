#pragma once
#include "library/Components/ProjectorComponent.h"

namespace library
{
class CameraComponent : public ProjectorComponent
{
	RTTI_CLASS(CameraComponent, ProjectorComponent)

public:
	CameraComponent();
	CameraComponent(
		const float nearPlaneDistance,
		const float farPlaneDistance,
		const float fieldOfView,
		const float aspectRatio
	);

protected:
	void InitializeInternal() override;
};
} // namespace library
