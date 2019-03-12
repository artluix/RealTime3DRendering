#pragma once
#include "library/Components/ProjectorComponent.h"

namespace library
{
	class PerspectiveProjectorComponent : public ProjectorComponent
	{
		RTTI_CLASS(PerspectiveProjectorComponent, ProjectorComponent)

	public:
		explicit PerspectiveProjectorComponent();
		explicit PerspectiveProjectorComponent(
			const float fieldOfView,
			const float aspectRatio,
			const float nearPlaneDistance,
			const float farPlaneDistance
		);

		float GetFieldOfView() const { return m_fieldOfView; }
		void SetFieldOfView(const float fieldOfView);

		float GetAspectRatio() const { return m_aspectRatio; }
		void SetAspectRatio(const float aspectRatio);

		bool UpdateProjectionMatrix() override;

		void Reset() override;

	protected:
		float m_fieldOfView;
		float m_aspectRatio;
	};
} // namespace library
