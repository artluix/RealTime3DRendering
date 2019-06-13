#pragma once
#include "library/Components/DrawableComponent.h"
#include "library/math/Color.h"
#include "library/Common.h"

#include <memory>
#include <optional>

namespace DirectX
{
class SpriteBatch;
} // namespace dx

namespace library
{
class UIComponent : public DrawableComponent
{
	RTTI_CLASS(UIComponent, DrawableComponent)

public:
	UIComponent();
	~UIComponent();

	void Update(const Time& time) override;
	void Draw(const Time& time) override;

	void SetTexture(ID3D11ShaderResourceView* const texture);
	ID3D11ShaderResourceView* GetTexture() { return m_texture; }

	void SetColor(const math::Color& color);
	const math::Color& GetColor() const { return m_color; }

	void SetDestinationRect(const RECT& rect);

protected:
	void InitializeInternal() override;

	void Begin();
	void End();

	std::unique_ptr<dx::SpriteBatch> m_spriteBatch;
	math::Color m_color;

private:
	ID3D11ShaderResourceView* m_texture;
	std::optional<RECT> m_destinationRect;
};
} // namespace library
