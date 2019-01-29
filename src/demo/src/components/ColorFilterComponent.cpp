#include "components/ColorFilterComponent.h"

#include <library/components/FullScreenQuadComponent.h>
#include <library/components/TextComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectVariable.h>

#include <library/Utils.h>
#include <library/Path.h>
#include <library/Application.h>

#include <sstream>

namespace demo
{
	using namespace library;

	namespace
	{
		constexpr float k_brightnessModulationRate = 3.0f;

		const auto k_effectPath = library::utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			library::Path("../data/effects/ColorFilter_d.fxc")
#else
			library::Path("../data/effects/ColorFilter.fxc")
#endif
		);
	}

	//-------------------------------------------------------------------------

	std::string ColorFilter::ToTechniqueName(const Type t)
	{
		switch (t)
		{
			case Grayscale:		return "grayscale_filter";
			case Inverse:		return "inverse_filter";
			case Sepia:			return "sepia_filter";
			case Generic:		return "generic_filter";
			default:			return "";
		}
	}

	std::string ColorFilter::ToString(const Type t)
	{
		switch (t)
		{
			case Grayscale:		return "Grayscale";
			case Inverse:		return "Inverse";
			case Sepia:			return "Sepia";
			case Generic:		return "Generic";
			default:			return "";
		}
	}

	ColorFilter::Type ColorFilter::Next(const Type t)
	{
		if (t == Last)
			return First;

		return static_cast<Type>(t + 1);
	}

	//-------------------------------------------------------------------------

	ColorFilterComponent::ColorFilterComponent()
		: m_colorFilterType(ColorFilter::First)
		, m_genericColorFilter(math::Matrix4::Identity)
	{
	}

	ColorFilterComponent::~ColorFilterComponent() = default;

	//-------------------------------------------------------------------------

	void ColorFilterComponent::Initialize(const Application& app)
	{
		InitializeMaterial(app, k_effectPath);
		InitializeQuad(app, "grayscale_filter");

		m_fullScreenQuad->SetMaterialUpdateFunction(
			[this]()
			{
				m_material->GetColorTexture() << GetSceneTexture();
				m_material->GetColorFilter() << m_genericColorFilter;
			}
		);

		m_text = std::make_unique<TextComponent>();
		m_text->SetPosition(math::Vector2(0.f, 45.f));
		m_text->SetTextGeneratorFunction(
			[this]() -> std::wstring
			{
				std::ostringstream oss;
				oss << "Color Filter (Space Bar): " << ColorFilter::ToString(m_colorFilterType) << "\n";

				if (m_colorFilterType == ColorFilter::Generic)
					oss << "Brightness (+Comma/-Period): " << m_genericColorFilter._11;

				return utils::ToWideString(oss.str());
			}
		);
		m_text->Initialize(app);
	}

	void ColorFilterComponent::Update(const Time& time)
	{
		if (m_keyboard->WasKeyPressed(library::Key::Space))
		{
			m_colorFilterType = ColorFilter::Next(m_colorFilterType);
			m_fullScreenQuad->SetActiveTechnique(ColorFilter::ToTechniqueName(m_colorFilterType));
		}

		if (m_colorFilterType == ColorFilter::Generic)
			UpdateGenericColorFilter(time);

		m_text->Update(time);
	}

	void ColorFilterComponent::Draw(const Time& time)
	{
		m_fullScreenQuad->Draw(time);
	}

	void ColorFilterComponent::UpdateGenericColorFilter(const library::Time& time)
	{
		static float brightness = 1.0f;

		if (!!m_keyboard)
		{
			const auto elapsedTime = time.elapsed.GetSeconds();

			if (m_keyboard->IsKeyDown(Key::Comma) && brightness < 1.f)
			{
				brightness += k_brightnessModulationRate * elapsedTime;
				brightness = math::Min(brightness, 1.f);
				m_genericColorFilter = math::Matrix4::Scaling(math::Vector3(brightness));
			}

			if (m_keyboard->IsKeyDown(Key::Period) && brightness > 0.f)
			{
				brightness -= k_brightnessModulationRate * elapsedTime;
				brightness = math::Max(brightness, 0.f);
				m_genericColorFilter = math::Matrix4::Scaling(math::Vector3(brightness));
			}
		}
	}
} // namespace demo
