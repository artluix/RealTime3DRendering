#pragma once
#include "library/components/BaseComponent.h"
#include "library/Color.h"

namespace library
{
	class LightComponent : public rtti::Class<LightComponent, BaseComponent>
	{
	public:
		explicit LightComponent(const Application& app);
		~LightComponent() = default;

		const Color& GetColor() const { return m_color; }
		void SetColor(const Color& color);

	private:
		Color m_color;
	};

} // namespace library
