#include "HdrDemo.h"

#include "DemoUtils.h"

#include <library/Components/TextComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Time.h>
#include <library/Utils.h>

#include <sstream>

using namespace library;

void HdrDemo::InitializeInternal()
{
	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 70.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::ostringstream oss;
		oss << "Gamma correction enabled (Space Bar): " << (IsGammaCorrectionEnabled() ? "true" : "false") << '\n'
			<< "Luminance sampling steps (+Up/-Down): " << GetLuminanceSamplingStepsCount() << '\n'
			<< "Luminance sampling mips count: " << GetLuminanceSamplingMipsCount() << '\n';
		return library::utils::ToWideString(oss.str());
	});
	m_text->Initialize(GetApp());

	ToneMappingComponent::InitializeInternal();

	SetLuminanceSamplingStepsCount(4);
}

//-------------------------------------------------------------------------

void HdrDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyReleased(Key::Space))
		{
			SetGammaCorrection(!IsGammaCorrectionEnabled());
		}

		int luminanceSamplingStepsCount = GetLuminanceSamplingStepsCount();
		const auto lumSamplingInterval = math::IntervalIntegral(1, GetLuminanceSamplingMipsCount() - 1);
		if (::utils::UpdateValueOnce(luminanceSamplingStepsCount, 1, lumSamplingInterval, *m_keyboard, KeyPair(Key::Up, Key::Down)))
		{
			SetLuminanceSamplingStepsCount(luminanceSamplingStepsCount);
		}
	}

	m_text->Update(time);
}
