#include "StdAfx.h"
#include "library/effects/Material.h"

#include "library/effects/Effect.h"
#include "library/effects/Technique.h"
#include "library/effects/Variable.h"
#include "library/effects/Pass.h"

#include "library/Model.h"
#include "library/Mesh.h"

#include <cassert>

namespace library
{
	namespace effects
	{
		Material::Material()
			: m_effect(nullptr)
			, m_currentTechnique(nullptr)
		{
		}

		Material::Material(const std::string& defaultTechniqueName)
			: m_effect(nullptr)
			, m_currentTechnique(nullptr)
			, m_defaultTechniqueName(defaultTechniqueName)
		{
		}

		Material::~Material() = default;

		Variable* Material::operator[](const std::string& variableName) const
		{
			return m_effect->GetVariable(variableName);
		}

		void Material::SetCurrentTechnique(Technique* const technique)
		{
			if (m_currentTechnique != technique)
			{
				m_currentTechnique = technique;
			}
		}

		ID3D11InputLayout* Material::GetInputLayout(Pass* const pass) const
		{
			const auto it = m_inputLayouts.find(pass);
			if (it != m_inputLayouts.cend())
				return it->second.Get();

			return nullptr;
		}

		void Material::Initialize(Effect* const effect)
		{
			if (m_effect != effect)
			{
				m_effect = effect;
				assert(!!m_effect);
			}

			Technique* defaultTechnique = nullptr;
			assert(m_effect->GetTechniquesCount() != 0);

			if (!m_defaultTechniqueName.empty())
			{
				defaultTechnique = m_effect->GetTechnique(m_defaultTechniqueName);
				assert(!!defaultTechnique);
			}
			else
			{
				defaultTechnique = m_effect->GetTechnique(0);
			}

			SetCurrentTechnique(defaultTechnique);
		}

		void Material::CreateInputLayout(
			const std::string& techniqueName,
			const std::string& passName,
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions
		)
		{
			auto technique = m_effect->GetTechnique(techniqueName);
			assert(!!technique);

			auto pass = technique->GetPass(passName);
			assert(!!technique);

			auto inputLayout = pass->CreateInputLayout(inputElementDescriptions);
		}

		std::vector<ComPtr<ID3D11Buffer>> Material::CreateVertexBuffers(ID3D11Device* const device, const Model& model) const
		{
			std::vector<ComPtr<ID3D11Buffer>> vertexBuffers;
			
			const auto meshesCount = model.GetMeshesCount();
			vertexBuffers.reserve(meshesCount);

			for (unsigned i = 0; i < meshesCount; i++)
			{
				auto mesh = model.GetMesh(i);
				auto vertexBuffer = CreateVertexBuffer(device, *mesh);
				vertexBuffers.push_back(vertexBuffer);
			}

			return vertexBuffers;
		}

	} // namespace effects
} // namespace library
