#include "StdAfx.h"
#include "library/Components/UIComponent.h"

#include "library/Application.h"
#include "library/Renderer.h"

namespace library
{
	namespace
	{
		const auto k_defaultColor = Color::White;
	}

	UIComponent::UIComponent()
		: m_texture(nullptr)
		, m_color(k_defaultColor)
	{
	}

	UIComponent::~UIComponent() = default;

	//-------------------------------------------------------------------------

	void UIComponent::Initialize(const Application& app)
	{
		DrawableComponent::Initialize(app);

		m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(app.GetDeviceContext());
	}

	void UIComponent::Update(const Time& time)
	{
		GetApp()->GetRenderer()->AddDrawable(*this);
	}

	void UIComponent::Draw(const Time& time)
	{
		if (m_texture)
		{
			Begin();

			if (m_destionationRect)
				m_spriteBatch->Draw(m_texture, m_destionationRect.value(), m_color.Load());

			End();
		}
	}

	//-------------------------------------------------------------------------

	void UIComponent::SetTexture(ID3D11ShaderResourceView* const texture)
	{
		m_texture = texture;
	}

	void UIComponent::SetColor(const Color& color)
	{
		m_color = color;
	}

	void UIComponent::SetDestinationRect(const RECT& rect)
	{
		m_destionationRect = std::make_optional(rect);
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
