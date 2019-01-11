#pragma once
#include "library/NonCopyable.hpp"

#include <vector>

// simple renderer to correct render order of components

namespace library
{
	class DrawableComponent;
	struct Time;

	class Renderer : public NonCopyable<Renderer>
	{
	public:
		using Drawable = DrawableComponent;

		explicit Renderer() = default;

		void RegisterForRender(Drawable* const drawable);
		void UnregisterForRender(Drawable* const drawable);

		void Render(const Time& time);

	private:
		using Drawables = std::vector<Drawable*>;

		void RenderByEffect(Drawables& drawables);

		Drawables m_drawables;
	};
} // namespace library
