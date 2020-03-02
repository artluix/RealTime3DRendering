#include "GaussianBlurDemo.h"

#include "DemoUtils.h"

#include <library/Components/KeyboardComponent.h>
#include <library/Components/TextComponent.h>
#include <library/Components/GaussianBlurComponent.h>

#include <library/Math/Math.h>
#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Application.h>

#include <cmath>
#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
constexpr float k_maxBlurAmount = 100.f;
constexpr float k_blurModulationRate = 1.0f;
constexpr auto k_backgroundColor = colors::Black;
} // namespace

//-------------------------------------------------------------------------

GaussianBlurDemo::GaussianBlurDemo() = default;
GaussianBlurDemo::~GaussianBlurDemo() = default;

//-------------------------------------------------------------------------

void GaussianBlurDemo::InitializeInternal()
{
	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 70.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << std::setprecision(2) << "Blur Amount (+J/-K): " << GetBlurAmount();

		return woss.str();
	});
	m_text->Initialize(GetApp());

	GaussianBlurComponent::InitializeInternal();
}

//-------------------------------------------------------------------------

void GaussianBlurDemo::Update(const Time& time)
{
	UpdateBlurAmount(time);

	m_text->Update(time);
}

void GaussianBlurDemo::UpdateBlurAmount(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto stepValue = time.elapsed.GetSeconds() * k_blurModulationRate;

		auto blurAmount = GetBlurAmount();
		if (::utils::UpdateValue(blurAmount, stepValue, math::Interval(0.f, k_maxBlurAmount), *m_keyboard, KeyPair(Key::Up, Key::Down)))
		{
			SetBlurAmount(blurAmount);
		}

	}
}
