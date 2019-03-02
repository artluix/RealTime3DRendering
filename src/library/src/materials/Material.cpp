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
	Material::Material(Effect& effect, const std::string& defaultTechniqueName /* = ""*/)
		: m_effect(effect)
		, m_currentTechnique(
			!defaultTechniqueName.empty() ?
			effect.GetTechnique(defaultTechniqueName) :
			effect.GetTechnique(0)
		)
	{
		assert(effect.IsInitialized());
	}

	EffectVariable& Material::operator[](const std::string& variableName)
	{
		return m_effect.GetVariable(variableName);
	}

	void Material::SetCurrentTechnique(const EffectTechnique& technique)
	{
		m_currentTechnique = technique;
	}

	//-------------------------------------------------------------------------

	ID3D11InputLayout* Material::GetInputLayout(const EffectPass& pass) const
	{
		const auto it = m_inputLayouts.find(&pass);
		if (it != m_inputLayouts.cend())
			return it->second.Get();

		return nullptr;
	}

	ComPtr<ID3D11InputLayout> Material::GetInputLayoutShared(const EffectPass& pass) const
	{
		const auto it = m_inputLayouts.find(&pass);
		if (it != m_inputLayouts.cend())
			return it->second;

		return ComPtr<ID3D11InputLayout>();
	}

	//-------------------------------------------------------------------------

	void Material::Initialize()
	{
		if (m_isInitialized)
			return;

		InitializeInternal();

		m_isInitialized = true;
	}

	void Material::CreateInputLayout(
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions,
		const std::string& techniqueName /* = "main11" */,
		const std::string& passName /* = "p0" */
	)
	{
		const auto& technique = m_effect.GetTechnique(techniqueName);
		const auto& pass = technique.GetPass(passName);
		auto inputLayout = pass.CreateInputLayout(inputElementDescriptions);

		m_inputLayouts.emplace(&pass, inputLayout);
	}

	void Material::CreateInputLayout(
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions,
		const EffectPass& pass
	)
	{
		auto inputLayout = pass.CreateInputLayout(inputElementDescriptions);
		m_inputLayouts.emplace(&pass, inputLayout);
	}

	std::vector<ComPtr<ID3D11Buffer>> Material::CreateVertexBuffers(
		ID3D11Device* const device,
		const Model& model
	) const
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

	//-------------------------------------------------------------------------

	ComPtr<ID3D11Buffer> Material::CreateVertexBuffer(
		ID3D11Device* const device,
		const void* data,
		const std::size_t size
	)
	{
		ComPtr<ID3D11Buffer> vertexBuffer;

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.ByteWidth = size;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = data;

		auto hr = device->CreateBuffer(
			&vertexBufferDesc,
			&vertexSubResourceData,
			vertexBuffer.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.");
		}

		return vertexBuffer;
	}
} // namespace library
