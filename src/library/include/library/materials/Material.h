#pragma once
#include "library/CommonTypes.h"
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"
#include "library/DirectXForwardDeclarations.h"

#include <string>
#include <map>
#include <vector>
#include <array>
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

		//-------------------------------------------------------------------------

		template<typename VertexType>
		static ComPtr<ID3D11Buffer> CreateVertexBuffer(
			ID3D11Device* const device,
			const std::vector<VertexType>& vertices
		);

		template<typename VertexType, std::size_t VerticesCount>
		static ComPtr<ID3D11Buffer> CreateVertexBuffer(
			ID3D11Device* const device,
			const std::array<VertexType, VerticesCount>& vertices
		);

		//-------------------------------------------------------------------------

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
		static ComPtr<ID3D11Buffer> CreateVertexBuffer(
			ID3D11Device* const device,
			const void* data,
			const std::size_t size
		);

		virtual void CreateInputLayout(const std::string& techniqueName, const std::string& passName = "p0");
		virtual void CreateInputLayout(const EffectPass& pass);
		void CreateInputLayout();

		virtual void InitializeInternal();

		Effect& m_effect;
		std::string m_defaultTechniqueName;

		std::reference_wrapper<const EffectTechnique> m_currentTechnique;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputElementDescriptions;
		std::map<const EffectPass*, ComPtr<ID3D11InputLayout>> m_inputLayouts;

	private:
		bool m_isInitialized = false;
	};

	//-------------------------------------------------------------------------

	template<typename VertexType>
	inline ComPtr<ID3D11Buffer> Material::CreateVertexBuffer(
		ID3D11Device* const device,
		const std::vector<VertexType>& vertices
	)
	{
		return CreateVertexBuffer(device, vertices.data(), vertices.size() * sizeof(VertexType));
	}

	template<typename VertexType, std::size_t VerticesCount>
	inline ComPtr<ID3D11Buffer> Material::CreateVertexBuffer(
		ID3D11Device* const device,
		const std::array<VertexType, VerticesCount>& vertices
	)
	{
		return CreateVertexBuffer(device, vertices.data(), VerticesCount * sizeof(VertexType));
	}
} // namespace library
