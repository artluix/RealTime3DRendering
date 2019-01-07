#include "StdAfx.h"
#include "library/components/MaterialComponent.h"

#include "library/materials/Material.h"

#include "library/effect/EffectPass.h"
#include "library/effect/EffectTechnique.h"

#include "library/Model.h"
#include "library/Mesh.h"

#include "library/Application.h"

namespace library
{
	MaterialComponent::MaterialComponent(const Application& app, const fs::Path& modelPath)
		: DrawableComponent(app)
		, m_modelPath(modelPath)
	{
	}

	void MaterialComponent::Initialize()
	{
		auto& material = GetMaterial();
		material.Initialize();

		const auto& app = GetApp();
		Model model(app, m_modelPath, true);

		const auto& mesh = model.GetMesh(0);
		m_vertexBuffer = material.CreateVertexBuffer(app.GetD3DDevice(), mesh);
		m_indexBuffer = mesh.CreateIndexBuffer();
		m_indicesCount = mesh.GetIndicesCount();
	}

	void MaterialComponent::Draw(const Time& time)
	{
		SetIA();
		UpdateVariables();
		Render();
	}

	void MaterialComponent::SetIA()
	{
		auto d3dDeviceContext = m_app->GetD3DDeviceContext();
		d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto& material = GetMaterial();

		auto& pass = material.GetCurrentTechnique().GetPass(0);
		auto inputLayout = material.GetInputLayout(pass);
		d3dDeviceContext->IASetInputLayout(inputLayout);

		unsigned stride = material.GetVertexSize();
		unsigned offset = 0;
		d3dDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		d3dDeviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void MaterialComponent::UpdateVariables()
	{
		auto& pass = GetMaterial().GetCurrentTechnique().GetPass(0);
		pass.Apply(0, m_app->GetD3DDeviceContext());
	}

	void MaterialComponent::Render()
	{
		m_app->GetD3DDeviceContext()->DrawIndexed(m_indicesCount, 0, 0);
	}

} // namespace library
