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
	}; // namespace filesystem

	class Application;

		class EffectTechnique;
		using TechniquePtr = std::shared_ptr<EffectTechnique>;

		class EffectVariable;
		using VariablePtr = std::shared_ptr<EffectVariable>;

	// ----------------------------------------------------------------------------------------------------------

	class Effect : public NonCopyable<Effect>
	{
	public:
		explicit Effect(const Application& app);
		~Effect() = default;

		static ComPtr<ID3DX11Effect> CompileFromFile(ID3D11Device* const device, const fs::Path& path);
		static ComPtr<ID3DX11Effect> LoadCompiledEffect(ID3D11Device* const device, const fs::Path& path);

		const Application& GetApp() const { return m_app; }

		ID3DX11Effect* const GetEffect() const { return m_effect.Get(); }
		void SetEffect(const ComPtr<ID3DX11Effect>& effect);

		const D3DX11_EFFECT_DESC& GetEffectDesc() const { return m_effectDesc; }

		EffectTechnique* GetTechnique(const std::string& techniqueName) const;
		EffectTechnique* GetTechnique(const unsigned techniqueIdx) const;
		std::size_t GetTechniquesCount() const { return m_techniques.size(); }

		EffectVariable* const GetVariable(const std::string& variableName) const;
		EffectVariable* const GetVariable(const unsigned variableIdx) const;
		std::size_t GetVariablesCount() const { return m_variables.size(); }

	private:
		void Initialize();

		const Application& m_app;

		ComPtr<ID3DX11Effect> m_effect;
		D3DX11_EFFECT_DESC m_effectDesc;

		std::vector<TechniquePtr> m_techniques;
		std::vector<VariablePtr> m_variables;

		std::map<std::string, TechniquePtr> m_techniquesMap;
		std::map<std::string, VariablePtr> m_variablesMap;
	};

	using EffectPtr = std::shared_ptr<Effect>;

} // namespace library
