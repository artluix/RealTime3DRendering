#pragma once
#include <library/Materials/DistortionMappingMaterial.h>

#include <library/Components/ConcreteMaterialPostProcessingComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/BufferData.h>

#include <optional>

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

	library::BufferData m_vertexBuffer;
	std::optional<library::BufferData> m_indices;

	float m_displacementScale;
	Mode m_mode;
};
