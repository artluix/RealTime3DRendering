#pragma once
#include "Materials/ComputeShaderMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.h>

namespace library
{
class TextComponent;
class FullScreenQuadComponent;
} // namespace library

class ComputeShaderDemo
	: public library::ConcreteMaterialPrimitiveComponent<ComputeShaderMaterial>
{
	RTTI_CLASS(ComputeShaderDemo, library::PrimitiveComponent)

public:
	ComputeShaderDemo();

	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

protected:
	void InitializeInternal() override;

private:
	struct Texture
	{
		enum Type : unsigned
		{
			Default = 0,

			//# Count
			Count
		};
	};

	std::unique_ptr<library::FullScreenQuadComponent> m_fullScreenQuad;
	std::unique_ptr<library::TextComponent> m_text;

	library::math::Vector2 m_textureSize;
	float m_blueColor;

	library::ComPtr<ID3D11UnorderedAccessView> m_outputTexture;

	library::math::Vector2u m_threadGroupCount;
};
