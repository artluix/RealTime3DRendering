#include "StdAfx.h"
#include "library/components/PrimitiveComponent.h"

#include "library/Render/Renderer.h"
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

void PrimitiveComponent::SetPrimitiveData(PrimitiveData&& pd, const unsigned idx)
{
	assert(idx < m_primitivesData.size());
	m_primitivesData[idx] = std::move(pd);
}

const PrimitiveData& PrimitiveComponent::GetPrimitiveData(const unsigned idx) const
{
	assert(idx < m_primitivesData.size());
	return m_primitivesData[idx];
}

//-------------------------------------------------------------------------

void PrimitiveComponent::AddPrimitiveData(PrimitiveData&& pd)
{
	m_primitivesData.emplace_back(std::move(pd));
}

void PrimitiveComponent::RemovePrimitiveData(const unsigned idx)
{
	assert(idx < m_primitivesData.size());
	m_primitivesData.erase(m_primitivesData.begin() + idx);
}

//-------------------------------------------------------------------------

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
		deviceContext->IASetVertexBuffers(
			0,
			1,
			&primitiveData.vertexBuffer.buffer,
			&primitiveData.vertexBuffer.stride,
			&primitiveData.vertexBuffer.offset
		);
	}

	// set index buffer (if needed)
	if (!!primitiveData.indexBuffer)
	{
		deviceContext->IASetIndexBuffer(primitiveData.indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
}

void PrimitiveComponent::Draw_Render(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	if (!!primitiveData.indexBuffer)
		deviceContext->DrawIndexed(primitiveData.indexBuffer->elementsCount, 0, 0);
	else
		deviceContext->Draw(primitiveData.vertexBuffer.elementsCount, 0);
}

} // namespace library
