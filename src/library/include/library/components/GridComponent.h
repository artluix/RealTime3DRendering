#pragma once
#include "library/components/SceneComponent.h"
#include "library/components/DrawableComponent.h"
#include "library/components/InputReceivableComponent.h"
#include "library/Color.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class GridComponent
		: public Scene
		, public DrawableComponent
		, public InputReceivableComponent
	{
		RTTI_CLASS(GridComponent, Scene, DrawableComponent, InputReceivableComponent)

	public:
		explicit GridComponent(const Application& app);
		explicit GridComponent(
			const Application& app,
			const unsigned size,
			const unsigned scale,
			const Color& color
		);

		~GridComponent() = default;

		unsigned GetSize() const { return m_size; }
		unsigned GetScale() const { return m_scale; }
		const Color& GetColor() const { return m_color; }

		void SetSize(const unsigned size);
		void SetScale(const unsigned scale);
		void SetColor(const Color& color);

		void Initialize() override;
		void Update(const Time& time) override;
		void Draw(const Time& time) override;

	private:
		void Build();

		unsigned m_size;
		unsigned m_scale;
		Color m_color;

		ComPtr<ID3DX11Effect> m_effect;
		ComPtr<ID3DX11EffectTechnique> m_technique;
		ComPtr<ID3DX11EffectPass> m_pass;
		ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11Buffer> m_vertexBuffer;
	};

} // namespace library
