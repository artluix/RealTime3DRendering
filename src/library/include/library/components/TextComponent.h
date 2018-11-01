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
}

namespace library
{
	namespace components
	{

		class TextComponent : public rtti::Class<TextComponent, UIComponent>
		{
		public:
			using TextGeneratorFunction = std::function<std::wstring(void)>;

			explicit TextComponent(const Application& app);
			~TextComponent();

			void SetText(const std::string& text);
			void SetText(const std::wstring& text);
			const std::wstring& GetText() const { return m_text; }

			void SetColor(const Color& color);
			const Color& GetColor() const { return m_color; }

			void SetFontPath(const filesystem::Path& fontPath);
			const filesystem::Path& GetPath() const { return m_fontPath; }

			void SetGeneratorFunction(const TextGeneratorFunction& generatorFunction);

			void Initialize() override;
			void Update(const Time& time) override;
			void Draw(const Time& time) override;

		private:
			static const Color k_defaultColor;
			static const filesystem::Path k_defaultFontPath;

			std::wstring m_text;
			Color m_color;
			filesystem::Path m_fontPath;

			TextGeneratorFunction m_generatorFunction;

			std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
			std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
		};

	} // namespace components
} // namespace library
