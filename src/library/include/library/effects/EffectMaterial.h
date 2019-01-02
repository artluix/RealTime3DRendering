#pragma once
#include "library/Common.h"
#include "library/DirectXForwardDeclarations.h"
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"

#include <string>
#include <map>
#include <vector>
#include <functional>

struct D3D11_INPUT_ELEMENT_DESC;

namespace library
{
	class Model;
	class Mesh;

	class Effect;
	class EffectVariable;
	class EffectTechnique;
	class EffectPass;

	// ----------------------------------------------------------------------------------------------------------

	class EffectMaterial
		: public rtti::Class<EffectMaterial>
		, public NonCopyable<EffectMaterial>
	{
	public:
		explicit EffectMaterial(Effect& effect, const std::string& defaultTechniqueName="");
		virtual ~EffectMaterial();

		EffectVariable& operator[](const std::string& variableName) const;
		Effect& GetEffect() const { return m_effect; }

		const EffectTechnique& GetCurrentTechnique() const { return m_currentTechnique; }
		void SetCurrentTechnique(const EffectTechnique& technique);

		ID3D11InputLayout* GetInputLayout(const EffectPass& pass) const;

		virtual void Initialize();

		virtual std::vector<ComPtr<ID3D11Buffer>> CreateVertexBuffers(ID3D11Device* const device, const Model& model) const;
		virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const = 0;

		virtual unsigned GetVertexSize() const = 0;

	protected:
		virtual void CreateInputLayout(
			const std::string& techniqueName,
			const std::string& passName,
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions
		);

	private:
		using EffectTechniqueRef = std::reference_wrapper<EffectTechnique>;
		using EffectTechniqueCRef = std::reference_wrapper<const EffectTechnique>;

		using EffectPassRef = std::reference_wrapper<EffectPass>;
		using EffectPassCRef = std::reference_wrapper<const EffectPass>;

		Effect& m_effect;
		EffectTechniqueCRef m_currentTechnique;

		std::string m_defaultTechniqueName;

		std::map<EffectPassCRef, ComPtr<ID3D11InputLayout>> m_inputLayouts;
	};

} // namespace library
