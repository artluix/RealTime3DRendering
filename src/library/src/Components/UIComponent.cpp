#include "StdAfx.h"
#include "library/Components/UIComponent.h"

#include "library/Application.h"
#include "library/Renderer.h"

namespace library
{
UIComponent::UIComponent()
	: m_texture(nullptr)
	, m_color(colors::White)
{}

UIComponent::~UIComponent() = default;

//-------------------------------------------------------------------------

void UIComponent::InitializeInternal()
{
	m_spriteBatch = std::make_unique<dx::SpriteBatch>(GetApp().GetDeviceContext());
}

void UIComponent::Update(const Time& time)
{
	GetApp().GetRenderer()->AddDrawable(*this);
}

void UIComponent::Draw(const Time& time)
{
	if (!m_texture || !m_destinationRect)
		return;

	Begin();
	m_spriteBatch->Draw(m_texture, m_destinationRect.value(), dx::VECTOR(m_color));
	End();
}

//-------------------------------------------------------------------------

void UIComponent::SetTexture(ID3D11ShaderResourceView* const texture)
{
	m_texture = texture;
}

void UIComponent::SetColor(const math::Color& color)
{
	m_color = color;
}

void UIComponent::SetDestinationRect(const RECT& rect)
{
	m_destinationRect = std::make_optional(rect);
}

//-------------------------------------------------------------------------

void UIComponent::Begin()
{
	m_spriteBatch->Begin();
}

void UIComponent::End()
{
	m_spriteBatch->End();
}

} // namespace library
