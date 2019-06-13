#include "StdAfx.h"
#include "library/Components/TextComponent.h"

#include "library/Application.h"
#include "library/Utils.h"

namespace library
{

namespace
{
const auto k_defaultFontPath =
	utils::GetExecutableDirectory().Join(Path("../data/fonts/Arial_14_Regular.spritefont"));
}

//-------------------------------------------------------------------------

TextComponent::TextComponent() : m_fontPath(k_defaultFontPath)
{}

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

void TextComponent::SetFontPath(const Path& fontPath)
{
	if (!m_spriteFont || m_fontPath != fontPath)
	{
		m_fontPath = fontPath;
		m_spriteFont =
			std::make_unique<DirectX::SpriteFont>(GetApp().GetDevice(), m_fontPath.GetWideCString());
	}
}

void TextComponent::SetTextUpdateFunction(const TextUpdateFunction& textUpdateFunction)
{
	m_textUpdateFunction = textUpdateFunction;
}

void TextComponent::InitializeInternal()
{
	UIComponent::InitializeInternal();

	SetFontPath(m_fontPath);
}

void TextComponent::Update(const Time& time)
{
	if (!!m_textUpdateFunction)
	{
		m_text = m_textUpdateFunction();
	}

	UIComponent::Update(time);
}

void TextComponent::Draw(const Time& time)
{
	Begin();

	m_spriteFont->DrawString(
		m_spriteBatch.get(),
		m_text.c_str(),
		dx::VECTOR(m_position),
		dx::VECTOR(m_color));

	End();
}
} // namespace library
