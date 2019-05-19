#include "BloomDemo.h"

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
}

//-------------------------------------------------------------------------

BloomDemo::BloomDemo() = default;
BloomDemo::~BloomDemo() = default;

//-------------------------------------------------------------------------

void BloomDemo::Initialize()
{
	m_text = std::make_unique<TextComponent>();
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			const auto& settings = GetSettings();

			std::wostringstream woss;
			woss <<
				std::setprecision(2) << L"Draw Mode (Enter): " << 
					utils::ToWideString(BloomDrawModeToString(GetDrawMode())) << "\n"

				<< L"Bloom Threshold (+U/-I): " << settings.bloomThreshold << "\n"
				<< L"Blur Amount (+J/-K): " << settings.blurAmount << "\n"
				<< L"Bloom Intensity (+N/-M): " << settings.bloomIntensity << "\n"
				<< L"Bloom Saturation (+G/-H): " << settings.bloomSaturation << "\n";
			return woss.str();
		}
	);
	m_text->SetPosition(math::Vector2(0.f, 200.f));
	m_text->Initialize(app);

	BloomComponent::Initialize(app);
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
	static auto settings = GetSettings();

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		// blur
		if (m_keyboard->IsKeyDown(Key::J))
		{
			settings.blurAmount += k_modulationRate * elapsedTime;
			SetSettings(settings);
		}

		if (m_keyboard->IsKeyDown(Key::K) && settings.blurAmount > 0.f)
		{
			settings.blurAmount -= k_modulationRate * elapsedTime;
			settings.blurAmount = math::Max(settings.blurAmount, 0.f);
			SetSettings(settings);
		}

		// bloom threshold
		if (m_keyboard->IsKeyDown(Key::U) && settings.bloomThreshold < 1.f)
		{
			settings.bloomThreshold += k_modulationRate * elapsedTime;
			settings.bloomThreshold = math::Min(settings.bloomThreshold, 1.f);
			SetSettings(settings);
		}

		if (m_keyboard->IsKeyDown(Key::I) && settings.bloomThreshold > 0.f)
		{
			settings.bloomThreshold -= k_modulationRate * elapsedTime;
			settings.bloomThreshold = math::Max(settings.bloomThreshold, 0.f);
			SetSettings(settings);
		}

		// bloom intensity
		if (m_keyboard->IsKeyDown(Key::N))
		{
			settings.bloomIntensity += k_modulationRate * elapsedTime;
			SetSettings(settings);
		}

		if (m_keyboard->IsKeyDown(Key::M) && settings.bloomIntensity > 0.f)
		{
			settings.bloomIntensity -= k_modulationRate * elapsedTime;
			settings.bloomIntensity = math::Max(settings.bloomIntensity, 0.f);
			SetSettings(settings);
		}

		// bloom saturation
		if (m_keyboard->IsKeyDown(Key::G))
		{
			settings.bloomSaturation += k_modulationRate * elapsedTime;
			SetSettings(settings);
		}

		if (m_keyboard->IsKeyDown(Key::H) && settings.bloomSaturation > 0.f)
		{
			settings.bloomSaturation -= k_modulationRate * elapsedTime;
			settings.bloomSaturation = math::Max(settings.bloomSaturation, 0.f);
			SetSettings(settings);
		}
	}
}
