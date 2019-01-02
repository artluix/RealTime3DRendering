#pragma once
#include "library/Common.h"
#include "library/DirectXForwardDeclarations.h"
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"

#include <string>
#include <map>
#include <vector>

struct D3D11_INPUT_ELEMENT_DESC;

namespace library
{
	class Model;
	class Mesh;

	class Effect;
	class EffectVariable;
	class EffectTechnique;
	class EffectPass;

	class EffectMaterial
		: public rtti::Class<EffectMaterial>
		, public NonCopyable<EffectMaterial>
	{
	public:
		explicit EffectMaterial();
		explicit EffectMaterial(const std::string& defaultTechniqueName);

		virtual ~EffectMaterial();

		EffectVariable* const operator[](const std::string& variableName) const;
		Effect* const GetEffect() const { return m_effect; }

		EffectTechnique* const GetCurrentTechnique() const { return m_currentTechnique; }
		void SetCurrentTechnique(EffectTechnique* const technique);

		ID3D11InputLayout* const GetInputLayout(EffectPass* const pass) const;

		virtual void Initialize(Effect* const effect);

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
		Effect* m_effect;

		EffectTechnique* m_currentTechnique;
		std::string m_defaultTechniqueName;

		std::map<EffectPass*, ComPtr<ID3D11InputLayout>> m_inputLayouts;
	};

} // namespace library
