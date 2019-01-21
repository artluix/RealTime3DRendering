#pragma once
#include "library/materials/Material.h"

#include "library/components/DrawableComponent.h"
#include "library/Path.h"
#include "library/DirectXForwardDeclarations.h"

#include <type_traits>
#include <memory>

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

		void Initialize() override;
		void Draw(const Time& time) override;

	protected:
		explicit MaterialComponent(const Application& app);

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

	//-------------------------------------------------------------------------

	class Effect;

	template<class MaterialType, typename = std::enable_if_t<std::is_base_of_v<Material, MaterialType>>>
	class ConcreteMaterialComponent : public MaterialComponent
	{
	public:
		using Material = MaterialType;

		const Material& GetMaterial() const override { return *m_material; }

	protected:
		explicit ConcreteMaterialComponent(const Application& app)
			: MaterialComponent(app)
		{
		}

		Material& GetMaterial() override { return *m_material; }

		std::shared_ptr<Effect> m_effect;
		std::unique_ptr<Material> m_material;
	};
} // namespace library
