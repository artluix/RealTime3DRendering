#include "HdrDemo.h"

#include "DemoUtils.h"

#include <library/Components/TextComponent.h>
#include <library/Components/FullScreenQuadComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Time.h>
#include <library/Utils.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectVariable.h>

#include <sstream>

using namespace library;

namespace
{
constexpr float k_modulationRate = 1.f;
}

//-------------------------------------------------------------------------

std::string HdrDemo::ToTechniqueName(const ToneMappingType t)
{
	switch (t)
	{
		case ToneMappingType::No:				return "no_tone_mapping";
		case ToneMappingType::Reinhard:			return "tone_mapping_reinhard";
		case ToneMappingType::Gamma:			return "tone_mapping_gamma";
		default:								return "";
	}
}

std::string HdrDemo::ToString(const ToneMappingType t)
{
	switch (t)
	{
	case ToneMappingType::No:				return "NoToneMapping";
	case ToneMappingType::Reinhard:			return "Reinhard";
	case ToneMappingType::Gamma:			return "Gamma";
	default:								return "";
	}
}

//-------------------------------------------------------------------------

HdrDemo::HdrDemo()
	: m_exposure(0.5f)
{
}

HdrDemo::~HdrDemo() = default;

//-------------------------------------------------------------------------

void HdrDemo::InitializeInternal()
{
	CreateMaterialWithEffect("ToneMapping");
	InitializeQuad(ToTechniqueName(ToneMappingType::No));

	m_fullScreenQuad->SetMaterialUpdateFunction([this]() {
		m_material->GetSceneTexture() << GetSceneTexture();
		m_material->GetExposure() << m_exposure;
	});

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 70.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::ostringstream oss;
		oss << "Tone Mapping (Space Bar): " << ToString(m_toneMappingType);

		if (m_toneMappingType == ToneMappingType::Gamma)
			oss << ", Exposure (+Up/-Down): " << m_exposure;

		oss << '\n';

		return library::utils::ToWideString(oss.str());
	});
	m_text->Initialize(GetApp());
}

void HdrDemo::Draw(const library::Time& time)
{
	m_fullScreenQuad->Draw(time);
}

//-------------------------------------------------------------------------

void HdrDemo::Update(const library::Time& time)
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Space))
		{
			if (m_toneMappingType == ToneMappingType::No)
				m_toneMappingType = ToneMappingType::Reinhard;
			else if (m_toneMappingType == ToneMappingType::Reinhard)
				m_toneMappingType = ToneMappingType::Gamma;
			else if (m_toneMappingType == ToneMappingType::Gamma)
				m_toneMappingType = ToneMappingType::No;

			m_fullScreenQuad->SetActiveTechnique(ToTechniqueName(m_toneMappingType));
		}
	}

	if (m_toneMappingType == ToneMappingType::Gamma)
		UpdateExposure(time);

	m_text->Update(time);
}

void HdrDemo::UpdateExposure(const library::Time& time)
{
	if (!!m_keyboard)
	{
		const auto stepValue = time.elapsed.GetSeconds() * k_modulationRate;
		::utils::UpdateValue(m_exposure, stepValue, math::Interval(0.f, 50.f), *m_keyboard, KeyPair(Key::Up, Key::Down));
	}
}
