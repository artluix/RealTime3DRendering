#include "StdAfx.h"
#include "library/components/MaterialComponent.h"

#include "library/effect/EffectPass.h"
#include "library/effect/EffectTechnique.h"

#include "library/materials/Material.h"

#include "library/Model.h"
#include "library/Mesh.h"

#include "library/Application.h"
#include "library/Exception.h"
#include "library/Path.h"
#include "library/Utils.h"

#include <cassert>

namespace library
{
	MaterialComponent::MaterialComponent(const Application& app)
		: DrawableComponent(app)
	{
	}

	//-------------------------------------------------------------------------

	void MaterialComponent::LoadModel(const Path& modelPath)
	{
		if (!modelPath)
			return;

		const auto& material = GetMaterial();
		assert(material.IsInitialized());

		Model model(m_app, modelPath, true);

		const auto& mesh = model.GetMesh(0);
		m_vertexBuffer = material.CreateVertexBuffer(m_app.GetDevice(), mesh);
		m_indexBuffer = mesh.CreateIndexBuffer();

		m_indicesCount = mesh.GetIndicesCount();
		m_verticesCount = mesh.GetVerticesCount();
	}

	//-------------------------------------------------------------------------

	void MaterialComponent::SetModelPath(const Path& path)
	{
		if (m_modelPath != path)
			m_modelPath = path;
	}

	void MaterialComponent::SetTexturePath(const Path& path)
	{
		if (m_texturePath != path)
			m_texturePath = path;
	}

	//-------------------------------------------------------------------------

	void MaterialComponent::Initialize()
	{
		LoadModel(m_modelPath);
		m_app.LoadTexture(m_texturePath, m_textureShaderResourceView);

		// set default input layout
		const auto& material = GetMaterial();

		const auto& currentTechnique = material.GetCurrentTechnique();
		const auto& pass = currentTechnique.GetPass(0);
		m_inputLayout = material.GetInputLayoutShared(pass);

		DrawableComponent::Initialize();
	}

	void MaterialComponent::Draw(const Time& time)
	{
		SetIA();
		SetEffectData();
		Render();
	}

	void MaterialComponent::SetIA()
	{
		const auto& material = GetMaterial();

		auto deviceContext = m_app.GetDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->IASetInputLayout(m_inputLayout.Get());

		if (!!m_vertexBuffer)
		{
			unsigned stride = material.GetVertexSize();
			unsigned offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		}

		if (!!m_indexBuffer)
		{
			deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		}
	}

	void MaterialComponent::SetEffectData()
	{
		auto& material = GetMaterial();

		auto& currentTechnique = material.GetCurrentTechnique();
		auto& pass = currentTechnique.GetPass(0);

		pass.Apply(0, m_app.GetDeviceContext());
	}

	void MaterialComponent::Render()
	{
		auto deviceContext = m_app.GetDeviceContext();

		if (m_indicesCount > 0)
			deviceContext->DrawIndexed(m_indicesCount, 0, 0);
		else
			deviceContext->Draw(m_verticesCount, 0);
	}
} // namespace library
