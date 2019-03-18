#include "StdAfx.h"
#include "library/Components/OrthographicProjectorComponent.h"

namespace library
{
	namespace
	{
		constexpr unsigned k_defaultWidth = 5;
		constexpr unsigned k_defaultHeight = 5;
		constexpr auto k_defaultNearPlaneDistance = 1.f;
		constexpr auto k_defaultFarPlaneDistance = 20.f;
	}

	//-------------------------------------------------------------------------

	OrthographicProjectorComponent::OrthographicProjectorComponent(
		const unsigned width,
		const unsigned height
	)
		: ProjectorComponent(k_defaultNearPlaneDistance, k_defaultFarPlaneDistance)
		, m_width(width)
		, m_height(height)
	{
	}

	OrthographicProjectorComponent::OrthographicProjectorComponent()
		: OrthographicProjectorComponent(k_defaultWidth, k_defaultHeight)
	{
	}

	//-------------------------------------------------------------------------

	void OrthographicProjectorComponent::SetWidth(const unsigned width)
	{
		if (m_width != width)
		{
			m_width = width;
			m_isProjectionMatrixDirty = true;
		}
	}

	void OrthographicProjectorComponent::SetHeight(const unsigned height)
	{
		if (m_height != height)
		{
			m_height = height;
			m_isProjectionMatrixDirty = true;
		}
	}

	//-------------------------------------------------------------------------

	bool OrthographicProjectorComponent::UpdateProjectionMatrix()
	{
		if (!m_isProjectionMatrixDirty)
			return false;

		m_projectionMatrix = math::Matrix4::OrthographicRH(
			static_cast<float>(m_width),
			static_cast<float>(m_height),
			m_nearPlaneDistance,
			m_farPlaneDistance
		);
		m_isProjectionMatrixDirty = false;

		return true;
	}

	void OrthographicProjectorComponent::Reset()
	{
		ProjectorComponent::Reset();

		m_width = k_defaultWidth;
		m_height = k_defaultHeight;
	}
} // namespace library
