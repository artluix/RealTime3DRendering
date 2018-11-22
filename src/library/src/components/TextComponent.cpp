#include "StdAfx.h"
#include "library/components/TextComponent.h"

#include "library/Application.h"
#include "library/Utils.h"

namespace library
{
	namespace components
	{
		namespace
		{
			constexpr auto k_defaultColor = colors::White;
			const auto k_defaultFontPath = utils::GetExecutableDirectory().Join(
				filesystem::Path("data/fonts/Arial_14_Regular.spritefont")
			);
		}

		TextComponent::TextComponent(const Application& app)
			: Class(app)
			, m_color(k_defaultColor)
			, m_fontPath(k_defaultFontPath)
		{
		}

		TextComponent::~TextComponent() = default;

		void TextComponent::SetText(const std::string& text)
		{
			SetText(utils::ToWideString(text));
		}

		void TextComponent::SetText(const std::wstring& text)
		{
			if (m_text != text)
			{
				m_text = text;
			}
		}

		void TextComponent::SetColor(const Color& color)
		{
			if (m_color != color)
			{
				m_color = color;
			}
		}

		void TextComponent::SetFontPath(const filesystem::Path& fontPath)
		{
			if (!m_spriteFont || m_fontPath != fontPath)
			{
				m_fontPath = fontPath;
				m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_app.GetD3DDevice(), m_fontPath.GetWideCString());
			}
		}

		void TextComponent::SetGeneratorFunction(const TextGeneratorFunction& generatorFunction)
		{
			m_generatorFunction = generatorFunction;
		}

		void TextComponent::Initialize()
		{
			m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_app.GetD3DDeviceContext());
			SetFontPath(m_fontPath);
		}

		void TextComponent::Update(const Time& time)
		{
			if (!!m_generatorFunction)
			{
				m_text = m_generatorFunction();
			}
		}

		void TextComponent::Draw(const Time& time)
		{
			m_spriteBatch->Begin();
			m_spriteFont->DrawString(m_spriteBatch.get(), m_text.c_str(), DirectX::XMFLOAT2(m_position), m_color.Load());
			m_spriteBatch->End();
		}
	} // namespace components
} // namespace library
