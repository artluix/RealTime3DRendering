#pragma once
#include "library/Components/UIComponent.h"
#include "library/Path.h"

#include <string>
#include <functional>

namespace DirectX
{
class SpriteFont;
} // namespace DirectX

namespace library
{
class TextComponent : public UIComponent
{
	RTTI_CLASS(TextComponent, UIComponent)

public:
	using TextUpdateFunction = std::function<std::wstring(void)>;

	TextComponent();
	~TextComponent();

	void SetText(const std::string& text);
	void SetText(const std::wstring& text);
	const std::wstring& GetText() const { return m_text; }

	void SetPosition(const math::Vector2& position);
	const math::Vector2& GetPosition() const { return m_position; }

	void SetFontPath(const Path& fontPath);
	const Path& GetFontPath() const { return m_fontPath; }

	void SetTextUpdateFunction(const TextUpdateFunction& textUpdateFunction);

	void Update(const Time& time) override;
	void Draw(const Time& time) override;

protected:
	void InitializeInternal() override;

	TextUpdateFunction m_textUpdateFunction;

	math::Vector2 m_position;
	std::wstring m_text;

private:
	Path m_fontPath;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
};
} // namespace library
