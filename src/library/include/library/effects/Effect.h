#pragma once
#include "library/Common.h"
#include "library/NonCopyable.hpp"

#include <d3dx11effect.h>
#include <vector>
#include <map>
#include <string>

interface ID3D11Device;

namespace library
{
	namespace fs
	{
		class Path;
	}; // namespace fs

	class Application;
	class EffectTechnique;
	class EffectVariable;

	// ----------------------------------------------------------------------------------------------------------

	class Effect : public NonCopyable<Effect>
	{
	public:
		explicit Effect(const Application& app);
		~Effect();

		static ComPtr<ID3DX11Effect> CompileFromFile(ID3D11Device* const device, const fs::Path& path);
		static ComPtr<ID3DX11Effect> LoadCompiledEffect(ID3D11Device* const device, const fs::Path& path);

		void CompileFromFile(const fs::Path& path);
		void LoadCompiledEffect(const fs::Path& path);

		const Application& GetApp() const { return m_app; }

		ID3DX11Effect* GetEffect() const { return m_effect.Get(); }
		void SetEffect(const ComPtr<ID3DX11Effect>& effect);

		const D3DX11_EFFECT_DESC& GetEffectDesc() const { return m_effectDesc; }

		bool HasTechnique(const std::string& techniqueName) const;
		EffectTechnique& GetTechnique(const std::string& techniqueName) const;
		EffectTechnique& GetTechnique(const unsigned techniqueIdx) const;
		std::size_t GetTechniquesCount() const { return m_techniques.size(); }

		bool HasVariable(const std::string& variableName) const;
		EffectVariable& GetVariable(const std::string& variableName) const;
		EffectVariable& GetVariable(const unsigned variableIdx) const;
		std::size_t GetVariablesCount() const { return m_variables.size(); }

	private:
		void Initialize();

		const Application& m_app;

		ComPtr<ID3DX11Effect> m_effect;
		D3DX11_EFFECT_DESC m_effectDesc;

		std::vector<EffectTechnique*> m_techniques;
		std::map<std::string, EffectTechnique*> m_techniquesMap;

		std::vector<EffectVariable*> m_variables;
		std::map<std::string, EffectVariable*> m_variablesMap;
	};

} // namespace library
