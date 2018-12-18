#pragma once
#include "library/Common.h"
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"

#include <string>
#include <map>
#include <vector>

interface ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

namespace library
{
	class Model;
	class Mesh;

	namespace effects
	{
		class Effect;
		class Variable;
		class Technique;
		class Pass;

		class Material
			: public rtti::Class<Material>
			, public NonCopyable<Material>
		{
		public:
			explicit Material();
			explicit Material(const std::string& defaultTechniqueName);

			virtual ~Material();

			Variable* operator[](const std::string& variableName) const;
			Effect* GetEffect() const { return m_effect; }
			
			Technique* GetCurrentTechnique() const { return m_currentTechnique; }
			void SetCurrentTechnique(Technique* const technique);

			ID3D11InputLayout* GetInputLayout(Pass* const pass) const;

			virtual void Initialize(Effect* const effect);

			virtual std::vector<ComPtr<ID3D11Buffer>> CreateVertexBuffers(ID3D11Device* const device, const Model& model) const;
			virtual ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const = 0;

			virtual unsigned GetVerticesCount() const = 0;

		protected:
			virtual void CreateInputLayout(
				const std::string& techniqueName,
				const std::string& passName,
				const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions
			);

		private:
			Effect* m_effect;

			Technique* m_currentTechnique;
			std::string m_defaultTechniqueName;

			std::map<Pass*, ComPtr<ID3D11InputLayout>> m_inputLayouts;
		};

	} // namespace effects
} // namespace library
