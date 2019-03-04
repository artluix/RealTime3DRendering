#include "GaussianBlurDemo.h"

#include <library/Components/KeyboardComponent.h>
#include <library/Components/TextComponent.h>
#include <library/Components/GaussianBlurComponent.h>

#include <library/Math/Math.h>
#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Color.h>
#include <library/Application.h>

#include <cmath>
#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
	constexpr float k_blurModulationRate = 1.0f;
	const auto k_backgroundColor = Color::Black;
}

//-------------------------------------------------------------------------

GaussianBlurDemo::GaussianBlurDemo() = default;
GaussianBlurDemo::~GaussianBlurDemo() = default;

//-------------------------------------------------------------------------

void GaussianBlurDemo::Initialize(const Application& app)
{
	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 45.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss << std::setprecision(2) << "Blur Amount (+J/-K): " << GetBlurAmount();

			return woss.str();
		}
	);
	m_text->Initialize(app);

	GaussianBlurComponent::Initialize(app);
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
		using namespace library;

		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::J))
		{
			const auto blurAmount = GetBlurAmount() + k_blurModulationRate * elapsedTime;
			SetBlurAmount(blurAmount);
		}

		if (m_keyboard->IsKeyDown(Key::K) && GetBlurAmount() > 0.f)
		{
			auto blurAmount = GetBlurAmount() - k_blurModulationRate * elapsedTime;
			blurAmount = math::Max(1e-10f, blurAmount);
			SetBlurAmount(blurAmount);
		}
	}
}
