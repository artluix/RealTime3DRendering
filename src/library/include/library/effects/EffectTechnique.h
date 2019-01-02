#pragma once
#include "library/Common.h"
#include "library/NonCopyable.hpp"

#include <d3dx11effect.h>
#include <vector>
#include <map>
#include <string>
#include <functional>

namespace library
{
	class Application;
	class Effect;
	class EffectPass;

	class EffectTechnique : public NonCopyable<EffectTechnique>
	{
	public:
		explicit EffectTechnique(const Application& app, Effect& effect, const ComPtr<ID3DX11EffectTechnique>& technique);
		~EffectTechnique() = default;

		Effect& GetEffect() { return m_effect; }
		const std::string& GetName() const { return m_name; }

		ID3DX11EffectTechnique* const GetTechnique() const { return m_technique.Get(); }
		const D3DX11_TECHNIQUE_DESC& GetTechniqueDesc() const { return m_techniqueDesc; }

		bool HasPass(const std::string& passName) const;
		EffectPass& GetPass(const std::string& passName) const;
		EffectPass& GetPass(const unsigned passIdx) const;
		std::size_t GetPassesCount() const { return m_passes.size(); }

	private:
		using EffectPassPtr = std::unique_ptr<EffectPass>;
		using EffectPassRef = std::reference_wrapper<EffectPass>;

		ComPtr<ID3DX11EffectTechnique> m_technique;
		D3DX11_TECHNIQUE_DESC m_techniqueDesc;

		Effect& m_effect;
		std::string m_name;

		std::vector<EffectPassPtr> m_passes;
		std::map<std::string, EffectPassRef> m_passesMap;
	};

	using EffectTechniqueRef = std::reference_wrapper<EffectTechnique>;
	using EffectTechniqueCRef = std::reference_wrapper<const EffectTechnique>;

} // namespace library
