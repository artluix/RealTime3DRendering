#pragma once
#include <library/materials/DistortionMappingMaterial.h>

#include <library/components/ConcreteMaterialPostProcessingComponent.hpp>
#include <library/components/InputReceivableComponent.h>

namespace library
{
	class TextComponent;

	class EffectPass;
} // namespace library

class DistortionMappingDemo
	: public library::ConcreteMaterialPostProcessingComponent<library::DistortionMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(DistortionMappingDemo, library::PostProcessingComponent, library::InputReceivableComponent)
public:
	enum class Mode
	{
		Fullscreen,
		Masking,
		MaskOnly
	};

	explicit DistortionMappingDemo();
	~DistortionMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

	static std::string ModeToString(const Mode mode);
	static Mode NextMode(const Mode mode);

private:
	void UpdateDisplacementScale(const library::Time& time);
	void UpdateDistortion();
	void UpdateDistortionMask();

	void DrawMeshForDistortionCutout();

	std::unique_ptr<library::TextComponent> m_text;

	ComPtr<ID3D11ShaderResourceView> m_distortionMapTexture;
	std::unique_ptr<library::FullScreenRenderTarget> m_cutoutRenderTarget;
	library::EffectPass* m_cutoutPass;

	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;

	unsigned m_indicesCount;

	float m_displacementScale;
	Mode m_mode;
};
