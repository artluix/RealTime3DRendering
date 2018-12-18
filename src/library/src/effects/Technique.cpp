#include "StdAfx.h"
#include "library/effects/Technique.h"

#include "library/Application.h"
#include "library/effects/Pass.h"
#include "library/effects/Effect.h"

namespace library
{
	namespace effects
	{
		Technique::Technique(const Application& app, Effect& effect, const ComPtr<ID3DX11EffectTechnique>& technique)
			: m_effect(effect)
			, m_technique(technique)
		{
			m_technique->GetDesc(&m_techniqueDesc);
			m_name = m_techniqueDesc.Name;

			for (unsigned i = 0; i < m_techniqueDesc.Passes; i++)
			{
				auto pass = std::make_shared<Pass>(app, *this, m_technique->GetPassByIndex(i));
				m_passes.push_back(pass);
				m_passesMap.insert(std::make_pair(pass->GetName(), pass));
			}
		}

	} // namespace effects
} // namespace library
