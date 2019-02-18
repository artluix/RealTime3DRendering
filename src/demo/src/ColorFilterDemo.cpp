#include "ColorFilterDemo.h"

#include <library/components/FullScreenQuadComponent.h>
#include <library/components/TextComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectVariable.h>

#include <library/Utils.h>
#include <library/Path.h>
#include <library/Application.h>

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
		case Grayscale:		return "grayscale_filter";
		case Inverse:		return "inverse_filter";
		case Sepia:			return "sepia_filter";
		case Generic:		return "generic_filter";
		default:				return "";
	}
}

std::string ColorFilterDemo::ToString(const Type t)
{
	switch (t)
	{
		case Grayscale:		return "Grayscale";
		case Inverse:		return "Inverse";
		case Sepia:			return "Sepia";
		case Generic:		return "Generic";
		default:				return "";
	}
}

ColorFilterDemo::Type ColorFilterDemo::Next(const Type t)
{
	if (t == Last)
		return First;

	return static_cast<Type>(t + 1);
}

//-------------------------------------------------------------------------

ColorFilterDemo::ColorFilterDemo()
	: m_type(ColorFilterDemo::First)
	, m_genericFilter(math::Matrix4::Identity)
{
}

ColorFilterDemo::~ColorFilterDemo() = default;

//-------------------------------------------------------------------------

void ColorFilterDemo::Initialize(const Application& app)
{
	InitializeMaterial(app, "ColorFilter");
	InitializeQuad(app, "grayscale_filter");

	m_fullScreenQuad->SetMaterialUpdateFunction(
		[this]()
		{
			m_material->GetSceneTexture() << GetSceneTexture();
			m_material->GetColorFilter() << m_genericFilter;
		}
	);

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 45.f));
	m_text->SetTextGeneratorFunction(
		[this]() -> std::wstring
		{
			std::ostringstream oss;
			oss << "Color Filter (Space Bar): " << ColorFilterDemo::ToString(m_type) << "\n";

			if (m_type == ColorFilterDemo::Generic)
				oss << "Brightness (+Comma/-Period): " << m_genericFilter._11;

			return utils::ToWideString(oss.str());
		}
	);
	m_text->Initialize(app);
}

void ColorFilterDemo::Update(const Time& time)
{
	if (m_keyboard->WasKeyPressed(Key::Space))
	{
		m_type = ColorFilterDemo::Next(m_type);
		m_fullScreenQuad->SetActiveTechnique(ColorFilterDemo::ToTechniqueName(m_type));
	}

	if (m_type == ColorFilterDemo::Generic)
		UpdateGenericFilter(time);

	m_text->Update(time);
}

void ColorFilterDemo::Draw(const Time& time)
{
	m_fullScreenQuad->Draw(time);
}

void ColorFilterDemo::UpdateGenericFilter(const Time& time)
{
	static float brightness = 1.0f;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Comma) && brightness < 1.f)
		{
			brightness += k_brightnessModulationRate * elapsedTime;
			brightness = math::Min(brightness, 1.f);
			m_genericFilter = math::Matrix4::Scaling(math::Vector3(brightness));
		}

		if (m_keyboard->IsKeyDown(Key::Period) && brightness > 0.f)
		{
			brightness -= k_brightnessModulationRate * elapsedTime;
			brightness = math::Max(brightness, 0.f);
			m_genericFilter = math::Matrix4::Scaling(math::Vector3(brightness));
		}
	}
}
