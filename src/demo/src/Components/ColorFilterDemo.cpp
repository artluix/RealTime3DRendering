#include "ColorFilterDemo.h"

#include "DemoUtils.h"

#include <library/Components/FullScreenQuadComponent.h>
#include <library/Components/TextComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectVariable.h>

#include <library/Utils.h>
#include <library/Path.h>
#include <library/Application.h>
#include <library/Math/Math.h>

#include <sstream>

using namespace library;

namespace
{
constexpr float k_brightnessModulationRate = 3.0f;
}

//-------------------------------------------------------------------------

std::string ColorFilterDemo::ToTechniqueName(const Type t)
{
	switch (t)
	{
		case Type::Grayscale:	return "grayscale_filter";
		case Type::Inverse:		return "inverse_filter";
		case Type::Sepia:		return "sepia_filter";
		case Type::Generic:		return "generic_filter";
		default:				return "";
	}
}

std::string ColorFilterDemo::ToString(const Type t)
{
	switch (t)
	{
		case Type::Grayscale:	return "Grayscale";
		case Type::Inverse:		return "Inverse";
		case Type::Sepia:		return "Sepia";
		case Type::Generic:		return "Generic";
		default:				return "";
	}
}

ColorFilterDemo::Type ColorFilterDemo::Next(const Type t)
{
	if (t == Type::Last)
		return Type::First;

	return static_cast<Type>(std::underlying_type_t<Type>(t) + 1);
}

//-------------------------------------------------------------------------

ColorFilterDemo::ColorFilterDemo() : m_type(Type::First), m_genericFilter(math::Matrix4::Identity)
{}

ColorFilterDemo::~ColorFilterDemo() = default;

//-------------------------------------------------------------------------

void ColorFilterDemo::InitializeInternal()
{
	CreateMaterialWithEffect("ColorFilter");
	InitializeQuad("grayscale_filter");

	m_fullScreenQuad->SetMaterialUpdateFunction([this]() {
		m_material->GetSceneTexture() << GetSceneTextureSRV();
		m_material->GetColorFilter() << m_genericFilter;
	});

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 70.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::ostringstream oss;
		oss << "Color Filter (Space Bar): " << ToString(m_type);

		if (m_type == Type::Generic)
			oss << ", Brightness (+Comma/-Period): " << m_genericFilter._11;

		oss << '\n';

		return library::utils::ToWideString(oss.str());
	});
	m_text->Initialize(GetApp());
}

void ColorFilterDemo::Update(const Time& time)
{
	if (m_keyboard->WasKeyPressed(Key::Space))
	{
		m_type = ColorFilterDemo::Next(m_type);
		m_fullScreenQuad->SetActiveTechnique(ToTechniqueName(m_type));
	}

	if (m_type == Type::Generic)
		UpdateGenericFilter(time);

	m_text->Update(time);
}

void ColorFilterDemo::Draw(const Time& time)
{
	m_fullScreenQuad->Draw(time);
}

void ColorFilterDemo::UpdateGenericFilter(const Time& time)
{
	if (!!m_keyboard)
	{
		const float stepValue = time.elapsed.GetSeconds() * k_brightnessModulationRate;
		float brightness = 1.f;
		if (::utils::UpdateValue(brightness, stepValue, math::UnitInterval, *m_keyboard, KeyPair(Key::Comma, Key::Period)))
		{
			m_genericFilter = math::Matrix4::Scaling(math::Vector3(brightness));
		}
	}
}
