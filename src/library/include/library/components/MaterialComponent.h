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
		explicit MaterialComponent(const Application& app, const fs::Path& modelPath);
		~MaterialComponent() = default;

		void Initialize() override;
		void Draw(const Time& time) override;

		virtual const Material& GetMaterial() const = 0;

	protected:
		virtual Material& GetMaterial() = 0;

		// render stages
		virtual void SetIA();
		virtual void UpdateVariables();
		virtual void Render();

		fs::Path m_modelPath;

		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;

		unsigned m_indicesCount;
	};

} // namespace library
