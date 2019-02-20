#pragma once
#include <library/materials/DistortionMappingMaterial.h>

#include <library/components/ConcreteMaterialPostProcessingComponent.hpp>
#include <library/components/InputReceivableComponent.h>

namespace library
{
	class TextComponent;
} // namespace library

class DistortionMappingDemo
	: public library::ConcreteMaterialPostProcessingComponent<library::DistortionMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(DistortionMappingDemo, library::MaterialPostProcessingComponent, library::InputReceivableComponent)
public:
	enum class Mode
	{
		Normal,
		Masking
	};

	explicit DistortionMappingDemo();
	~DistortionMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

private:
	void UpdateDisplacementScale(const library::Time& time);

	void UpdateDistortionMapMaterial();

	static Mode NextMode(const Mode mode);

	std::unique_ptr<library::FullScreenRenderTarget> m_cutoutRenderTarget;

	std::unique_ptr<library::TextComponent> m_text;

	ComPtr<ID3D11ShaderResourceView> m_distortionMapTexture;

	float m_displacementScale;
	Mode m_mode;
};
