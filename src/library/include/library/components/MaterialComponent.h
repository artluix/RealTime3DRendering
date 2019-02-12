#pragma once
#include "library/components/DrawableComponent.h"

#include "library/CommonTypes.h"
#include "library/DirectXForwardDeclarations.h"

#include <string>

namespace library
{
	class Material;

	class MaterialComponent : public DrawableComponent
	{
		RTTI_CLASS(MaterialComponent, DrawableComponent)

	public:
		const std::string& GetModelName() const { return m_modelName; }
		void SetModelName(const std::string& modelName);

		const std::string& GetTextureName() const { return m_textureName; }
		void SetTextureName(const std::string& textureName);

		void Initialize(const Application& app) override;
		void Draw(const Time& time) override;

		virtual const Material& GetMaterial() const = 0;

	protected:
		explicit MaterialComponent() = default;

		virtual Material& GetMaterial() = 0;

		// render stages
		virtual void SetIA();
		virtual void SetEffectData();
		virtual void Render();

		void LoadModel(const std::string& modelName);

		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11ShaderResourceView> m_textureShaderResourceView;

		unsigned m_verticesCount = 0;
		unsigned m_indicesCount = 0;

	private:
		std::string m_modelName;
		std::string m_textureName;
	};
} // namespace library
