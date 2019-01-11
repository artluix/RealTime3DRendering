#pragma once
#include "library/components/UIComponent.h"
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
	class TextComponent : public UIComponent
	{
		RTTI_CLASS(TextComponent, UIComponent)

	public:
		using TextGeneratorFunction = std::function<std::wstring(void)>;

		explicit TextComponent(const Application& app);
		~TextComponent();

		void SetText(const std::string& text);
		void SetText(const std::wstring& text);
		const std::wstring& GetText() const { return m_text; }

		void SetColor(const Color& color);
		const Color& GetColor() const { return m_color; }

		void SetFontPath(const Path& fontPath);
		const Path& GetFontPath() const { return m_fontPath; }

		void SetTextGeneratorFunction(const TextGeneratorFunction& generatorFunction);

		void Initialize() override;
		void Update(const Time& time) override;
		void Draw(const Time& time) override;

	private:
		TextGeneratorFunction m_generatorFunction;
		std::wstring m_text;
		Color m_color;
		Path m_fontPath;
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	};
} // namespace library
