#pragma once
#include <library/materials/ColorFilterMaterial.h>

#include <library/components/ConcretePostProcessingComponent.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/math/Matrix.h>
#include <library/DirectXForwardDeclarations.h>

namespace library
{
	class TextComponent;
} // namespace library

namespace demo
{
	struct ColorFilter
	{
		enum Type
		{
			Grayscale,
			Inverse,
			Sepia,
			Generic,

			First = Grayscale,
			Last = Generic,
		};

		static std::string ToTechniqueName(const Type t);
		static std::string ToString(const Type t);

		static Type Next(const Type t);
	};

	//-------------------------------------------------------------------------

	class ColorFilterComponent
		: public library::ConcretePostProcessingComponent<library::ColorFilterMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(ColorFilterComponent, library::PostProcessingComponent, library::InputReceivableComponent)

	public:
		explicit ColorFilterComponent();
		~ColorFilterComponent();

		void Initialize(const library::Application& app) override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		void UpdateGenericColorFilter(const library::Time& time);

		ColorFilter::Type m_colorFilterType;

		library::math::Matrix4 m_genericColorFilter;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
