#pragma once
#include "library/Components/DrawableComponent.h"
#include "library/Math/Color.h"
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

	void SetTextureSRV(ID3D11ShaderResourceView* const textureSRV);
	ID3D11ShaderResourceView* GetTextureSRV() { return m_textureSRV; }

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
	ID3D11ShaderResourceView* m_textureSRV;
	std::optional<RECT> m_destinationRect;
};
} // namespace library
