#pragma once
#include "library/components/DrawableComponent.h"

#include "library/Path.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class Material;

	class MaterialComponent : public DrawableComponent
	{
		RTTI_CLASS(MaterialComponent, DrawableComponent)

	public:
		const Path& GetModelPath() const { return m_modelPath; }
		void SetModelPath(const Path& path);

		const Path& GetTexturePath() const { return m_texturePath; }
		void SetTexturePath(const Path& path);

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

		void LoadModel(const Path& modelPath);

		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11ShaderResourceView> m_textureShaderResourceView;

		unsigned m_verticesCount = 0;
		unsigned m_indicesCount = 0;

	private:
		Path m_modelPath;
		Path m_texturePath;
	};
} // namespace library
