#pragma once
#include "library/components/DrawableComponent.h"
#include "library/Color.h"
#include "library/Path.h"

#include <string>
#include <functional>

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
} // namespace DirectX

namespace library
{
	class TextComponent : public DrawableComponent
	{
		RTTI_CLASS(TextComponent, DrawableComponent)

	public:
		using TextUpdateFunction = std::function<std::wstring(void)>;

		explicit TextComponent();
		~TextComponent();

		void SetText(const std::string& text);
		void SetText(const std::wstring& text);
		const std::wstring& GetText() const { return m_text; }

		void SetPosition(const math::Vector2& position);
		const math::Vector2& GetPosition() const { return m_position; }

		void SetColor(const Color& color);
		const Color& GetColor() const { return m_color; }

		void SetFontPath(const Path& fontPath);
		const Path& GetFontPath() const { return m_fontPath; }

		void SetTextUpdateFunction(const TextUpdateFunction& textUpdateFunction);

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;
		void Draw(const Time& time) override;

	protected:
		TextUpdateFunction m_textUpdateFunction;

		math::Vector2 m_position;
		std::wstring m_text;
		Color m_color;

	private:
		Path m_fontPath;
		
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	};
} // namespace library
