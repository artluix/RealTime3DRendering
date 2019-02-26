#pragma once
#include "library/CommonTypes.h"
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"
#include "library/DirectXForwardDeclarations.h"

#include <string>
#include <map>
#include <vector>
#include <memory>
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

	class Material : public NonCopyable<Material>
	{
		RTTI_CLASS_BASE(Material)

	public:
		explicit Material(Effect& effect, const std::string& defaultTechniqueName = "");
		virtual ~Material() = default;

		static ComPtr<ID3D11Buffer> CreateVertexBuffer(
			ID3D11Device* const device,
			const void* data,
			const std::size_t size
		);

		EffectVariable& operator[](const std::string& variableName);
		const Effect& GetEffect() const { return m_effect; }

		const EffectTechnique& GetCurrentTechnique() const { return m_currentTechnique; }
		void SetCurrentTechnique(const EffectTechnique& technique);

		ID3D11InputLayout* GetInputLayout(const EffectPass& pass) const;
		ComPtr<ID3D11InputLayout> GetInputLayoutShared(const EffectPass& pass) const;

		void Initialize();
		bool IsInitialized() const { return m_isInitialized; }

		virtual std::vector<ComPtr<ID3D11Buffer>> CreateVertexBuffers(
			ID3D11Device* const device,
			const Model& model
		) const;

		virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(
			ID3D11Device* const device,
			const Mesh& mesh
		) const = 0;

		virtual unsigned GetVertexSize() const = 0;

	protected:
		virtual void CreateInputLayout(
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions,
			const std::string& techniqueName = "main11",
			const std::string& passName = "p0"
		);

		virtual void CreateInputLayout(
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions,
			const EffectPass& pass
		);

		virtual void InitializeInternal() = 0;

		Effect& m_effect;

		std::string m_defaultTechniqueName;
		std::reference_wrapper<const EffectTechnique> m_currentTechnique;

		std::map<const EffectPass*, ComPtr<ID3D11InputLayout>> m_inputLayouts;

	private:
		bool m_isInitialized = false;
	};
} // namespace library
