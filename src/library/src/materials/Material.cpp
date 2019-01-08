#include "StdAfx.h"
#include "library/materials/Material.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectVariable.h"
#include "library/effect/EffectPass.h"

#include "library/Model.h"
#include "library/Mesh.h"
#include "library/Exception.h"

namespace library
{
	Material::Material(const std::string& defaultTechniqueName /* = ""*/)
		: m_defaultTechniqueName(defaultTechniqueName)
	{
	}

	EffectVariable* Material::operator[](const std::string& variableName) const
	{
		return m_effect->GetVariable(variableName);
	}

	void Material::SetCurrentTechnique(const EffectTechnique& technique)
	{
		if (m_currentTechnique != &technique)
		{
			m_currentTechnique = &technique;
		}
	}

	ID3D11InputLayout* Material::GetInputLayout(const EffectPass& pass) const
	{
		const auto it = m_inputLayouts.find(&pass);
		if (it != m_inputLayouts.cend())
			return it->second.Get();

		return nullptr;
	}

	void Material::Initialize(const Effect& effect)
	{
		if (m_effect != &effect)
		{
			m_effect = &effect;

			EffectTechnique* currentTechnique = nullptr;

			if (!m_defaultTechniqueName.empty())
				currentTechnique = effect.GetTechnique(m_defaultTechniqueName);
			else
				currentTechnique = effect.GetTechnique(0);

			assert(!!currentTechnique);
			SetCurrentTechnique(*currentTechnique);
		}
	}

	void Material::CreateInputLayout(
		const std::string& techniqueName,
		const std::string& passName,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions
	)
	{
		const auto technique = m_effect->GetTechnique(techniqueName);
		assert(!!technique);

		const auto pass = technique->GetPass(passName);
		assert(!!pass);

		auto inputLayout = pass->CreateInputLayout(inputElementDescriptions);

		m_inputLayouts.emplace(&pass, inputLayout);
	}

	std::vector<ComPtr<ID3D11Buffer>> Material::CreateVertexBuffers(ID3D11Device* const device, const Model& model) const
	{
		std::vector<ComPtr<ID3D11Buffer>> vertexBuffers;

		const auto meshesCount = model.GetMeshesCount();
		vertexBuffers.reserve(meshesCount);

		for (unsigned i = 0; i < meshesCount; i++)
		{
			const auto& mesh = model.GetMesh(i);
			vertexBuffers.push_back(CreateVertexBuffer(device, mesh));
		}

		return vertexBuffers;
	}

	ComPtr<ID3D11Buffer> Material::CreateVertexBuffer(ID3D11Device* const device, const void* data, const std::size_t size) const
	{
		ComPtr<ID3D11Buffer> vertexBuffer;

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.ByteWidth = size;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = data;

		auto hr = device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.");
		}

		return vertexBuffer;
	}

} // namespace library
