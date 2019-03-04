#pragma once
#include "library/Components/DrawableComponent.h"

#include "library/Color.h"

#include <memory>
#include <optional>

namespace DirectX
{
	class SpriteBatch;
} // namespace DirectX

namespace library
{
	class UIComponent : public DrawableComponent
	{
		RTTI_CLASS(UIComponent, DrawableComponent)

	public:
		explicit UIComponent();
		~UIComponent();

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;
		void Draw(const Time& time) override;

		void SetTexture(ID3D11ShaderResourceView* const texture);
		ID3D11ShaderResourceView* GetTexture() { return m_texture; }

		void SetColor(const Color& color);
		const Color& GetColor() const { return m_color; }

		void SetDestinationRect(const RECT& rect);

	protected:
		void Begin();
		void End();

		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		Color m_color;

	private:
		ID3D11ShaderResourceView* m_texture;
		std::optional<RECT> m_destionationRect;
	};
} // namespace library
