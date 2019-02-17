#include "StdAfx.h"
#include "library/components/DrawableInputComponent.h"

#include "library/Application.h"

#include <cassert>

namespace library
{
	void DrawableInputComponent::SetDefaultTextureName(const std::string& textureName)
	{
		if (m_defaultTextureName != textureName)
			m_defaultTextureName = textureName;
	}

	//-------------------------------------------------------------------------

	void DrawableInputComponent::Initialize(const Application& app)
	{
		DrawableComponent::Initialize(app);

		if (!m_defaultTextureName.empty())
		{
			m_defaultTexture = m_app->LoadTexture(m_defaultTextureName);
		}
	}

	//-------------------------------------------------------------------------

	void DrawableInputComponent::Draw_SetIA()
	{
		auto deviceContext = m_app->GetDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->IASetInputLayout(m_inputLayout.Get());

		// set vertex buffer
		{
			unsigned stride = GetVertexSize();
			unsigned offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, m_vertexBufferData.buffer.GetAddressOf(), &stride, &offset);
		}

		// set index buffer (if needed)
		if (!!m_indexBufferData)
		{
			deviceContext->IASetIndexBuffer(m_indexBufferData->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		}
	}

	void DrawableInputComponent::Draw_SetData()
	{
	}

	void DrawableInputComponent::Draw_Render()
	{
		auto deviceContext = m_app->GetDeviceContext();

		if (!!m_indexBufferData)
			deviceContext->DrawIndexed(m_indexBufferData->count, 0, 0);
		else
			deviceContext->Draw(m_vertexBufferData.count, 0);
	}

	//-------------------------------------------------------------------------

	void DrawableInputComponent::Draw(const Time& time)
	{
		Draw_SetIA();
		Draw_SetData();
		Draw_Render();
	}
} // namespace library
