#pragma once
#include "library/Components/ProjectorComponent.h"

namespace library
{
	class OrthographicProjectorComponent : public ProjectorComponent
	{
		RTTI_CLASS(OrthographicProjectorComponent, ProjectorComponent)

	public:
		explicit OrthographicProjectorComponent();
		explicit OrthographicProjectorComponent(const unsigned width, const unsigned height);

		unsigned GetWidth() const { return m_width; }
		void SetWidth(const unsigned width);

		unsigned GetHeight() const { return m_height; }
		void SetHeight(const unsigned height);

		bool UpdateProjectionMatrix() override;

		void Reset() override;

	protected:
		unsigned m_width;
		unsigned m_height;
	};
} // namespace library
