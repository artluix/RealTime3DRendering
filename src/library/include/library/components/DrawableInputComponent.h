#pragma once
#include "library/components/DrawableComponent.h"

#include "library/CommonTypes.h"
#include "library/DirectXForwardDeclarations.h"

#include <string>
#include <memory>

namespace library
{
	class DrawableInputComponent : public virtual DrawableComponent
	{
		RTTI_CLASS(DrawableInputComponent, DrawableComponent)

	public:
		const std::string& GetDefaultTextureName() const { return m_defaultTextureName; }
		void SetDefaultTextureName(const std::string& textureName);

		void Initialize(const Application& app) override;
		void Draw(const Time& time) override;

		virtual unsigned GetVertexSize() const = 0;

	protected:
		struct BufferData
		{
			ComPtr<ID3D11Buffer> buffer;
			unsigned count = 0;
		};

		explicit DrawableInputComponent() = default;

		// draw stages
		virtual void Draw_SetIA();
		virtual void Draw_SetData();
		virtual void Draw_Render();

		BufferData m_vertexBufferData; // mandatory
		std::unique_ptr<BufferData> m_indexBufferData; // optional
		ComPtr<ID3D11InputLayout> m_inputLayout;

		ComPtr<ID3D11ShaderResourceView> m_defaultTexture;

	private:
		std::string m_defaultTextureName;
	};
} // namespace library
