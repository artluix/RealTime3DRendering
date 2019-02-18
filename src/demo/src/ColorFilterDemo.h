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

class ColorFilterDemo
	: public library::ConcretePostProcessingComponent<library::ColorFilterMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ColorFilterDemo, library::PostProcessingComponent, library::InputReceivableComponent)

public:
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

	//-------------------------------------------------------------------------

	explicit ColorFilterDemo();
	~ColorFilterDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

private:
	void UpdateGenericFilter(const library::Time& time);

	Type m_type;
	library::math::Matrix4 m_genericFilter;
	std::unique_ptr<library::TextComponent> m_text;
};