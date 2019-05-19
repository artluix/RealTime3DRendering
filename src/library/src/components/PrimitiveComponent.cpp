#include "StdAfx.h"
#include "library/components/PrimitiveComponent.h"

#include "library/Renderer.h"
#include "library/Application.h"

#include "library/Materials/Material.h"

#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectPass.h"

#include <cassert>

namespace library
{
PrimitiveComponent::PrimitiveComponent() = default;
PrimitiveComponent::~PrimitiveComponent() = default;

//-------------------------------------------------------------------------

ID3D11InputLayout* PrimitiveComponent::GetInputLayout() const
{
	if (GetMaterial())
		return m_inputLayout;

	return m_dxInputLayout.Get();
}

ID3D11ShaderResourceView* PrimitiveComponent::GetTexture(const unsigned textureIdx) const
{
	assert(textureIdx < m_textures.size());
	return m_textures[textureIdx].Get();
}

//-------------------------------------------------------------------------

void PrimitiveComponent::Update(const Time& time)
{
	SceneComponent::Update(time);

	GetApp().GetRenderer()->AddDrawable(*this);
}

void PrimitiveComponent::Draw(const Time& time)
{
	if (m_primitivesData.empty())
		return;

	for (const auto& pd : m_primitivesData)
	{
		Draw_SetIA(pd);
		Draw_SetData(pd);
		Draw_Render(pd);
	}
}

//-------------------------------------------------------------------------

void PrimitiveComponent::Draw_SetIA(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	deviceContext->IASetPrimitiveTopology(primitiveData.primitiveTopology);
	deviceContext->IASetInputLayout(GetInputLayout());

	// set vertex buffer
	{
		unsigned offset = 0;
		deviceContext
			->IASetVertexBuffers(0, 1, &primitiveData.vertexBuffer.buffer, &primitiveData.stride, &offset);
	}

	// set index buffer (if needed)
	if (!!primitiveData.indexBuffer)
	{
		deviceContext->IASetIndexBuffer(primitiveData.indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}

void PrimitiveComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	if (GetMaterial())
	{
		m_effectPass->Apply(0, deviceContext);
	}
	else
	{
		m_dxEffectPass->Apply(0, deviceContext);
	}
}

void PrimitiveComponent::Draw_Render(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	if (!!primitiveData.indexBuffer)
		deviceContext->DrawIndexed(static_cast<UINT>(primitiveData.indexBuffer->elementsCount), 0, 0);
	else
		deviceContext->Draw(static_cast<UINT>(primitiveData.vertexBuffer.elementsCount), 0);
}
} // namespace library
