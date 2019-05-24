#pragma once
#include "library/Components/SceneComponent.h"
#include "library/Math/Matrix.h"

namespace library
{
enum class ProjectionType
{
	Perspective,
	Orthographic
};

//-------------------------------------------------------------------------

class ProjectorComponent : public SceneComponent
{
	RTTI_CLASS(ProjectorComponent, SceneComponent)

public:
	explicit ProjectorComponent(const ProjectionType projectionType = ProjectionType::Perspective);
	ProjectorComponent(
		const float nearPlaneDistance,
		const float farPlaneDistance,
		const float fieldOfView,
		const float aspectRatio);

	ProjectorComponent(
		const float nearPlaneDistance,
		const float farPlaneDistance,
		const unsigned int width,
		const unsigned int height);

	~ProjectorComponent();

	//-------------------------------------------------------------------------

	ProjectionType GetProjectionType() const { return m_projectionType; }
	void SetProjectionType(const ProjectionType projectionType);

	float GetNearPlaneDistance() const { return m_nearPlaneDistance; }
	void SetNearPlaneDistance(const float nearPlaneDistance);

	float GetFarPlaneDistance() const { return m_farPlaneDistance; }
	void SetFarPlaneDistance(const float farPlaneDistance);

	//-------------------------------------------------------------------------

	float GetFieldOfView() const;
	void SetFieldOfView(const float fieldOfView);

	float GetAspectRatio() const;
	void SetAspectRatio(const float aspectRatio);

	//-------------------------------------------------------------------------

	unsigned int GetWidth() const;
	void SetWidth(const unsigned int width);

	unsigned int GetHeight() const;
	void SetHeight(const unsigned int height);

	//-------------------------------------------------------------------------

	const math::Matrix4& GetViewMatrix() const;
	const math::Matrix4& GetProjectionMatrix() const;
	const math::Matrix4& GetViewProjectionMatrix() const;

	void Update(const Time& time) override;

	// void Reset();
protected:
	void InitializeInternal() override;

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void UpdateViewProjectionMatrix();

	void OnTranslation(const math::Vector3& translationDelta) override;
	void OnRotation(const math::Quaternion& rotationDelta) override;

	float m_nearPlaneDistance;
	float m_farPlaneDistance;

	union
	{
		struct
		{
			float m_fieldOfView, m_aspectRatio;
		};

		struct
		{
			unsigned int m_width, m_height;
		};
	};

	ProjectionType m_projectionType;

	math::Matrix4 m_viewMatrix = math::Matrix4::Identity;
	bool m_isViewMatrixDirty = true;

	math::Matrix4 m_projectionMatrix = math::Matrix4::Identity;
	bool m_isProjectionMatrixDirty = true;

	math::Matrix4 m_viewProjectionMatrix = math::Matrix4::Identity;
};
} // namespace library
