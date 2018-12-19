#pragma once
#include "library/Common.h"
#include "library/NonCopyable.hpp"

#include <d3dx11effect.h>
#include <vector>
#include <map>
#include <string>

namespace library
{
	class Application;

	namespace effects
	{
		class Effect;

		class Pass;
		using PassPtr = std::shared_ptr<Pass>;

		class Technique : public NonCopyable<Technique>
		{
		public:
			explicit Technique(const Application& app, Effect& effect, const ComPtr<ID3DX11EffectTechnique>& technique);

			~Technique() = default;

			Effect& GetEffect() { return m_effect; }
			const std::string& GetName() const { return m_name; }

			ID3DX11EffectTechnique* const GetTechnique() const { return m_technique.Get(); }
			const D3DX11_TECHNIQUE_DESC& GetTechniqueDesc() const { return m_techniqueDesc; }

			Pass* const GetPass(const std::string& passName) const;
			Pass* const GetPass(const unsigned passIdx) const;
			std::size_t GetPassesCount() const { return m_passes.size(); }

		private:
			ComPtr<ID3DX11EffectTechnique> m_technique;
			D3DX11_TECHNIQUE_DESC m_techniqueDesc;

			Effect& m_effect;
			std::string m_name;

			std::vector<PassPtr> m_passes;
			std::map<std::string, PassPtr> m_passesMap;
		};

		using TechniquePtr = std::shared_ptr<Technique>;

	} // namespace effects
} // namespace library
