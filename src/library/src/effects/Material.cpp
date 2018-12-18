#include "StdAfx.h"
#include "library/effects/Material.h"

#include "library/effects/Effect.h"
#include "library/effects/Technique.h"
#include "library/effects/Variable.h"

#include "library/Model.h"
#include "library/Mesh.h"

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
			const auto& variables = m_effect->GetVariablesMap();
			const auto it = variables.find(variableName);
			if (it != variables.end())
				return it->second.get();

			return nullptr;
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

		}

		std::vector<ComPtr<ID3D11Buffer>> Material::CreateVertexBuffers(ID3D11Device* const device, const Model& model) const
		{

		}

		void Material::CreateInputLayout(
			const std::string& techniqueName,
			const std::string& passName,
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions
		)
		{

		}

	} // namespace effects
} // namespace library
