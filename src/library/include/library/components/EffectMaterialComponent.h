#pragma once
#include "library/components/DrawableComponent.h"
#include "library/fs/Path.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	namespace effect
	{
		class Material;
	} // namespace effect

	namespace components
	{
		class EffectMaterial : public DrawableComponent
		{
			RTTI_CLASS(EffectMaterial, DrawableComponent)

		public:
			void Initialize() override;
			void Draw(const Time& time) override;

			virtual const effect::Material& GetMaterial() const = 0;

		protected:
			explicit EffectMaterial(const Application& app, const fs::Path& modelPath);

			virtual effect::Material& GetMaterial() = 0;

			// render stages
			virtual void SetIA();
			virtual void SetEffectData();
			virtual void Render();

			fs::Path m_modelPath;

			ComPtr<ID3D11Buffer> m_indexBuffer;
			ComPtr<ID3D11Buffer> m_vertexBuffer;

			unsigned m_indicesCount;
		};

	} // namespace components
} // namespace library
