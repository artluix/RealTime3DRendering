#pragma once
#include "Common.h"
#include "NonCopyable.hpp"

#include <d3dx11effect.h>
#include <vector>
#include <map>
#include <string>

struct ID3D11Device;

namespace library
{
	namespace filesystem
	{
		class Path;
	}; // namespace filesystem

	class Application;

	class Effect : public NonCopyable<Effect>
	{
	public:
		explicit Effect(const Application& app);
		~Effect() = default;

		static ComPtr<ID3DX11Effect> CompileFromFile(ID3D11Device* const d3dDevice, const filesystem::Path& path);
		static void LoadCompiledEffect(ID3D11Device* const d3dDevice, const filesystem::Path& path);

		const Application& GetApp() const { return m_app; }

		ID3DX11Effect* const GetEffect() const { return m_effect.Get(); }
		void SetEffect(const ComPtr<ID3DX11Effect>& effect);

		const D3DX11_EFFECT_DESC& GetEffectDesc() const { return m_effectDesc; }

		//const std::vector<TechniquePtr>& GetTechniques() const { return m_techniques; }
		//const std::vector<VariablePtr>& GetVariables() const { return m_variables; }

		//const std::map<TechniquePtr>& GetTechniquesMap() const { return m_techniquesMap; }
		//const std::map<VariablePtr>& GetVariablesMap() const { return m_variablesMap; }

	private:
		void Initialize();

		const Application& m_app;

		ComPtr<ID3DX11Effect> m_effect;
		D3DX11_EFFECT_DESC m_effectDesc;

		//std::vector<TechniquePtr> m_techiques;
		//std::vector<VariablePtr> m_variables;

		//std::map<std::string, TechniquePtr> m_techniquesMap;
		//std::map<std::string, VariablePtr> m_variablesMap;

	};

} // namespace library
