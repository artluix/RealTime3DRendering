#pragma once
#include "library/components/DrawableComponent.h"

namespace library
{
	class MaterialComponent : public DrawableComponent
	{
		RTTI_CLASS(MaterialComponent, DrawableComponent)

	public:
		virtual const Material& GetMaterial() const = 0;

		const Path& GetModelPath() const { return m_modelPath; }
		void SetModelPath(const Path& path);

		const Path& GetTexturePath() const { return m_texturePath; }
		void SetTexturePath(const Path& path);

	protected:
		explicit MaterialComponent(const Application& app);

	private:
		virtual Material& GetMaterial() = 0;

		// render stages
		virtual void SetIA();
		virtual void SetEffectData();
		virtual void Render();

		void LoadModel(const Path& modelPath, ComPtr<ID3D11Buffer>& vertexBuffer, ComPtr<ID3D11Buffer>& indexBuffer);
		void LoadTexture(const Path& texturePath, ComPtr<ID3D11ShaderResourceView>& textureShaderResourceView);

		Path m_modelPath;
		Path m_texturePath;

		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11InputLayout> m_inputLayout;

		ComPtr<ID3D11ShaderResourceView> m_textureShaderResourceView;

		unsigned m_verticesCount = 0;
		unsigned m_indicesCount = 0;
	};
} // namespace library
