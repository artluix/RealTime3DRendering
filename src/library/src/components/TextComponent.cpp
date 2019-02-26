#include "StdAfx.h"
#include "library/components/TextComponent.h"

#include "library/Application.h"
#include "library/Renderer.h"
#include "library/Utils.h"

namespace library
{
	namespace
	{
		const auto k_defaultColor = Color::White;
		const auto k_defaultFontPath =
			utils::GetExecutableDirectory().Join(Path("../data/fonts/Arial_14_Regular.spritefont"));
	}

	//-------------------------------------------------------------------------

	TextComponent::TextComponent()
		: m_color(k_defaultColor)
		, m_fontPath(k_defaultFontPath)
	{
	}

	TextComponent::~TextComponent() = default;

	//-------------------------------------------------------------------------

	void TextComponent::SetText(const std::string& text)
	{
		SetText(utils::ToWideString(text));
	}

	void TextComponent::SetText(const std::wstring& text)
	{
		m_text = text;
	}

	void TextComponent::SetPosition(const math::Vector2& position)
	{
		m_position = position;
	}

	void TextComponent::SetColor(const Color& color)
	{
		m_color = color;
	}

	void TextComponent::SetFontPath(const Path& fontPath)
	{
		if (!m_spriteFont || m_fontPath != fontPath)
		{
			m_fontPath = fontPath;
			m_spriteFont = std::make_unique<DirectX::SpriteFont>(
				GetApp()->GetDevice(),
				m_fontPath.GetWideCString()
			);
		}
	}

	void TextComponent::SetTextUpdateFunction(const TextUpdateFunction& textUpdateFunction)
	{
		m_textUpdateFunction = textUpdateFunction;
	}

	void TextComponent::Initialize(const Application& app)
	{
		DrawableComponent::Initialize(app);

		m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(app.GetDeviceContext());
		SetFontPath(m_fontPath);
	}

	void TextComponent::Update(const Time& time)
	{
		if (!!m_textUpdateFunction)
		{
			m_text = m_textUpdateFunction();
		}

		GetApp()->GetRenderer()->AddDrawable(*this);
	}

	void TextComponent::Draw(const Time& time)
	{
		m_spriteBatch->Begin();
		m_spriteFont->DrawString(m_spriteBatch.get(), m_text.c_str(), GetPosition().Load(), m_color.Load());
		m_spriteBatch->End();
	}
} // namespace library
