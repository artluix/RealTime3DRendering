#include "BloomDemo.h"

#include "DemoUtils.h"

#include <library/Components/TextComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Utils.h>
#include <library/Time.h>
#include <library/Math/Math.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
constexpr float k_modulationRate = 1.0f;
constexpr float k_maxInf = 100.f;
}

//-------------------------------------------------------------------------

BloomDemo::BloomDemo() = default;
BloomDemo::~BloomDemo() = default;

//-------------------------------------------------------------------------

void BloomDemo::InitializeInternal()
{
	m_text = std::make_unique<TextComponent>();
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		const auto& settings = GetSettings();

		std::wostringstream woss;
		woss << std::setprecision(2) << L"Draw Mode (Enter): "
			 << library::utils::ToWideString(BloomDrawModeToString(GetDrawMode())) << "\n"

			 << L"Bloom Threshold (+U/-I): " << settings.bloomThreshold << "\n"
			 << L"Blur Amount (+J/-K): " << settings.blurAmount << "\n"
			 << L"Bloom Intensity (+N/-M): " << settings.bloomIntensity << "\n"
			 << L"Bloom Saturation (+F/-H): " << settings.bloomSaturation << "\n";
		return woss.str();
	});
	m_text->SetPosition(math::Vector2(0.f, 200.f));
	m_text->Initialize(GetApp());

	BloomComponent::InitializeInternal();
}

void BloomDemo::Update(const Time& time)
{
	if (!!m_keyboard && m_keyboard->WasKeyPressed(Key::Return))
	{
		m_drawMode = BloomDrawModeNext(m_drawMode);
	}

	UpdateSettings(time);

	m_text->Update(time);
}

void BloomDemo::UpdateSettings(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();
		const float stepValue = elapsedTime * k_modulationRate;

		auto settings = GetSettings();

		bool settingsUpdated = false;
		settingsUpdated |= ::utils::UpdateValue(settings.blurAmount, stepValue, math::Interval(.0f, k_maxInf), *m_keyboard, KeyPair(Key::J, Key::K));
		settingsUpdated |= ::utils::UpdateValue(settings.bloomThreshold, stepValue, math::UnitInterval, *m_keyboard, KeyPair(Key::U, Key::I));
		settingsUpdated |= ::utils::UpdateValue(settings.bloomIntensity, stepValue, math::Interval(.0f, k_maxInf), *m_keyboard, KeyPair(Key::N, Key::M));
		settingsUpdated |= ::utils::UpdateValue(settings.bloomSaturation, stepValue, math::Interval(.0f, k_maxInf), *m_keyboard, KeyPair(Key::F, Key::H));

		if (settingsUpdated)
			SetSettings(settings);
	}
}
